/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloimusa <aloimusa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:58:35 by aloimusa          #+#    #+#             */
/*   Updated: 2025/07/21 18:58:40 by aloimusa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void		exit_after_children(t_pipe *p);
static t_pipe	*init_struct(int ac);

int	main(int ac, char **av, char **env)
{
	t_pipe	*p;

	if (ac != 5)
		exit(EXIT_SUCCESS);
	check_files(av, ac);
	p = init_struct(ac);
	while (++p->child < ac - 3)
	{
		if (pipe(p->fd[NEXT]) == -1)
			exit(EXIT_FAILURE);
		p->pid[p->child] = fork();
		if (p->pid[p->child] == -1)
			exit(EXIT_FAILURE);
		if (p->pid[p->child] == 0)
		{
			init_pipes(ac, av, p);
			if (p->child != ac - 4)
				dup2(p->fd[NEXT][WRITE], STDOUT_FILENO);
			close(p->fd[NEXT][WRITE]);
			execute(av, env, free_pipex(NULL, p));
		}
		cleanup(ac, p);
	}
	exit_after_children(p);
}

char	*str_to_bin(char *bin, char **env)
{
	char	*ptr;
	char	**array;
	int		i;

	if (exec_cases(bin, env) == 1)
		return (ft_strdup(bin));
	ptr = ft_strnstr(*env, "PATH=", 5);
	while (!ptr && *++env)
		ptr = ft_strnstr(*env, "PATH=", 5);
	if (!ptr)
		return (NULL);
	array = ft_split(ptr + 5, ':');
	if (!array)
		return (NULL);
	i = -1;
	while (array[++i])
	{
		ptr = ft_strapp(ft_strjoin(array[i], "/"), bin);
		if (access(ptr, X_OK) == 0)
			break ;
		free(ptr);
		ptr = NULL;
	}
	free_pipex(array, NULL);
	return (ptr);
}

int	free_pipex(char **array, t_pipe *p)
{
	int	i;
	int	child;

	if (array)
	{
		i = -1;
		while (array[++i])
			free(array[i]);
		free(array);
	}
	if (p)
	{
		i = -1;
		if (p->pid)
			free(p->pid);
		child = p->child;
		free(p);
		return (child);
	}
	return (0);
}

static void	exit_after_children(t_pipe *p)
{
	int	i;
	int	status;
	int	last_status;

	i = -1;
	while (++i < p->child - 1)
		waitpid(p->pid[i], &status, 0);
	last_status = 0;
	waitpid(p->pid[p->child - 1], &last_status, 0);
	free_pipex(NULL, p);
	if (WIFEXITED(last_status))
		exit(WEXITSTATUS(last_status));
	else if (WIFSIGNALED(last_status))
		exit(128 + WTERMSIG(last_status));
	else
		exit(1);
}

static t_pipe	*init_struct(int ac)
{
	t_pipe	*p;

	p = ft_calloc(1, sizeof(t_pipe));
	if (!p)
		exit(code("pointer"));
	p->pid = malloc((ac - 3) * sizeof(int));
	p->child = -1;
	return (p);
}
