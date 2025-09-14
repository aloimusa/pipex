/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloimusa <aloimusa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 18:45:18 by aloimusa          #+#    #+#             */
/*   Updated: 2025/09/14 18:45:19 by aloimusa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	exit_after_children(t_pipe p);
static char	*get_path2(char **prog, char *ptr);

int	main(int ac, char **av, char **env)
{
	t_pipe	p;

	if (ac < 5)
		exit(0);
	p.pid = ft_calloc((ac - 3), sizeof(int));
	p.child = -1;
	while (++p.child < ac - 3)
	{
		if (pipe(p.fd[NEXT]) == -1)
			exit_error("pipe failed\n", p.pid, FREE_OBJ, 2);
		p.pid[p.child] = fork();
		if (p.pid[p.child] == -1)
			exit_error("fork failed\n", p.pid, FREE_OBJ, 2);
		if (p.pid[p.child] == 0)
			child(ac, av, env, p);
		if (p.child != 0)
		{
			close(p.fd[LAST][WRITE]);
			close(p.fd[LAST][READ]);
		}
		p.fd[LAST][WRITE] = p.fd[NEXT][WRITE];
		p.fd[LAST][READ] = p.fd[NEXT][READ];
	}
	exit_after_children(p);
}

static void	exit_after_children(t_pipe p)
{
	int	i;
	int	status;
	int	last_status;

	close(p.fd[LAST][WRITE]);
	close(p.fd[LAST][READ]);
	i = -1;
	while (++i < p.child - 1)
		waitpid(p.pid[i], &status, 0);
	last_status = 0;
	waitpid(p.pid[p.child - 1], &last_status, 0);
	free(p.pid);
	if (WIFEXITED(last_status))
		exit(WEXITSTATUS(last_status));
	else if (WIFSIGNALED(last_status))
		exit(128 + WTERMSIG(last_status));
	else
		exit(1);
}

char	*get_path(char **prog, char **env)
{
	char	*ptr;

	if (access(prog[0], X_OK) == 0)
		return (ft_strdup(prog[0]));
	ptr = ft_strnstr(*env, "PATH=", 5);
	while (!ptr && *++env)
		ptr = ft_strnstr(*env, "PATH=", 5);
	if (!ptr || ft_strncmp(prog[0], "./", 2) == 0)
	{
		ptr = ft_strdup(prog[0]);
		free_array(prog);
		if (!ptr)
			exit_error("malloc failed\n", NULL, NO, 2);
		if (errno == ENOENT)
			exit_error("%s: no such executable\n", ptr, FREE_OBJ, 127);
		else if (errno == EPERM || errno == EACCES)
			exit_error("%s: no executable permissions\n", ptr, FREE_OBJ, 126);
		else
			exit_error("%s: not executable\n", ptr, FREE_OBJ, 126);
	}
	return (get_path2(prog, ptr));
}

static char	*get_path2(char **prog, char *ptr)
{
	int		i;
	char	**array;

	array = ft_split(ptr + 5, ':');
	if (!array)
		if (free_array(prog))
			exit_error("malloc failed\n", NULL, NO, 2);
	i = -1;
	while (array[++i])
	{
		ptr = ft_strapp(ft_strjoin(array[i], "/"), prog[0]);
		if (access(ptr, X_OK) == 0)
			break ;
		free(ptr);
		ptr = NULL;
	}
	free_array(array);
	return (ptr);
}

int	free_array(char **array)
{
	int	i;

	if (array)
	{
		i = -1;
		while (array[++i])
			free(array[i]);
		free(array);
	}
	return (1);
}
