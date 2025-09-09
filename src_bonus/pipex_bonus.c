/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloimusa <aloimusa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 19:55:47 by aloimusa          #+#    #+#             */
/*   Updated: 2025/08/07 19:55:49 by aloimusa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	execute(char **av, char **env, int child)
{
	char	**array;
	char	*ptr;

	array = ft_split(av[child + 2], ' ');
	if (!array)
		exit(code("array"));
	if (!array[0] || !array[0][0])
		if (free_pipex(array, NULL) > -1)
			ft_error("empty string aren't executable\n", NULL, 0, 127);
	ptr = str_to_bin(array[0], env);
	if (!ptr)
	{
		ft_fprintf(2, "%s isn't executable\n", array[0]);
		free_pipex(array, NULL);
		exit(127);
	}
	free(array[0]);
	array[0] = ptr;
	execve(array[0], array, env);
	free_pipex(array, NULL);
	exit(EXIT_FAILURE);
}

void	cleanup(int ac, t_pipe *p)
{
	if (p->child != 0)
		close(p->fd[LAST][READ]);
	if (p->child == ac - 4)
		close(p->fd[NEXT][READ]);
	close(p->fd[NEXT][WRITE]);
	p->fd[LAST][WRITE] = p->fd[NEXT][WRITE];
	p->fd[LAST][READ] = p->fd[NEXT][READ];
}

void	init_pipes(int ac, char **av, t_pipe *p)
{
	if (p->child == 0)
	{
		p->fd[IOFD][IN] = open(av[1], O_RDONLY);
		if (p->fd[IOFD][IN] < 0)
			if (free_pipex(NULL, p) > -1)
				exit(code(av[1]));
		dup2(p->fd[IOFD][IN], STDIN_FILENO);
		close(p->fd[IOFD][IN]);
	}
	else if (dup2(p->fd[LAST][READ], STDIN_FILENO))
		if (close(p->fd[LAST][READ]) > -2)
			close(p->fd[LAST][WRITE]);
	if (p->child == ac - 4)
	{
		p->fd[IOFD][OUT] = open(av[ac - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (p->fd[IOFD][OUT] < 0 && (errno == EACCES || errno == EPERM))
			if (free_pipex(NULL, p) > -1)
				exit(1);
		if (p->fd[IOFD][OUT] < 0)
			if (free_pipex(NULL, p) > -1)
				exit(code(av[ac - 1]));
		dup2(p->fd[IOFD][OUT], STDOUT_FILENO);
		close(p->fd[IOFD][OUT]);
		close(p->fd[NEXT][READ]);
	}
}

int	code(void *str)
{
	if (errno == ENOENT)
		ft_fprintf(2, "%s ain't no file\n", str);
	else if (errno == EACCES)
		ft_fprintf(2, "you don't have permissions for %s\n", str);
	else if (errno == EPERM)
		ft_fprintf(2, "you don't have permissions for %s\n", str);
	else if (errno == ENOEXEC)
		ft_fprintf(2, "%s isn't excecutable\n", str);
	else if (errno == EISDIR)
		ft_fprintf(2, "%s is a directory\n", str);
	else if (errno == ENOMEM)
		ft_fprintf(2, "ran out of memory allocating for a %s\n", str);
	else if (errno == ENOSPC)
		ft_fprintf(2, "no space for %s\n", str);
	else
		ft_fprintf(2, "undefined error with one mr.%s\n", str);
	if (errno == ENOENT)
		return (127);
	else if (errno == EACCES || errno == ENOEXEC)
		return (126);
	else
		return (1);
}

void	check_files(char **av, int ac)
{
	int	in;
	int	out;

	in = open(av[1], O_RDONLY);
	if (in >= 0)
		close(in);
	else
		ft_fprintf(2, "no infile called %s\n", av[1]);
	out = open(av[ac - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (out >= 0)
		close(out);
	if (in < 0 && out < 0)
	{
		code(av[ac - 1]);
		exit(1);
	}
}
