/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloimusa <aloimusa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 09:20:58 by aloimusa          #+#    #+#             */
/*   Updated: 2025/09/12 09:21:22 by aloimusa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	first_child(char **av, int fd[3][2]);
static void	last_child(char **av, int fd[3][2], int child);
static void	execute(char **av, char **env, int child);

void	child(int ac, char **av, char **env, t_pipe p)
{
	free(p.pid);
	if (p.child == 0)
		first_child(av, p.fd);
	else if (p.child == ac - 4)
		last_child(av, p.fd, p.child);
	else
	{
		close(p.fd[LAST][WRITE]);
		if (dup2(p.fd[LAST][READ], STDIN_FILENO) == -1)
		{
			close(p.fd[NEXT][READ]);
			close(p.fd[NEXT][WRITE]);
			close(p.fd[LAST][READ]);
			exit_error("dup2 failed\n", NULL, NO, 2);
		}
		close(p.fd[LAST][READ]);
		if (dup2(p.fd[NEXT][WRITE], STDOUT_FILENO) == -1)
		{
			close(p.fd[NEXT][READ]);
			close(p.fd[NEXT][WRITE]);
			exit_error("dup2 failed\n", NULL, NO, 2);
		}
		close(p.fd[NEXT][WRITE]);
	}
	execute(av, env, p.child);
}

static void	first_child(char **av, int fd[3][2])
{
	if (dup2(fd[NEXT][WRITE], STDOUT_FILENO) == -1)
	{
		close(fd[NEXT][WRITE]);
		close(fd[NEXT][READ]);
		exit_error("dup2 failed\n", NULL, NO, 2);
	}
	close(fd[NEXT][WRITE]);
	fd[IOFD][IN] = open(av[1], O_RDONLY);
	if (fd[IOFD][IN] < 0)
		exit_error(NULL, av[1], OPEN_FAIL, NO);
	if (dup2(fd[IOFD][IN], STDIN_FILENO) == -1)
	{
		close(fd[IOFD][IN]);
		exit_error("dup2 failed\n", NULL, NO, 2);
	}
	close(fd[IOFD][IN]);
}

static void	last_child(char **av, int fd[3][2], int child)
{
	close(fd[LAST][WRITE]);
	close(fd[NEXT][WRITE]);
	close(fd[NEXT][READ]);
	if (dup2(fd[LAST][READ], STDIN_FILENO) == -1)
	{
		close(fd[LAST][READ]);
		exit_error("dup2 failed\n", NULL, NO, 2);
	}
	close(fd[LAST][READ]);
	fd[IOFD][OUT] = open(av[child + 3], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd[IOFD][OUT] < 0)
		exit_error(NULL, av[child + 3], OPEN_FAIL, NO);
	if (dup2(fd[IOFD][OUT], STDOUT_FILENO) == -1)
	{
		close(fd[IOFD][OUT]);
		exit_error("dup2 failed\n", NULL, NO, 2);
	}
	close(fd[IOFD][OUT]);
}

static void	execute(char **av, char **env, int child)
{
	char	**array;
	char	*ptr;

	array = ft_split(av[child + 2], ' ');
	if (!array)
		exit_error("malloc failed\n", NULL, NO, 2);
	if (!array[0] || !array[0][0])
		if (free_array(array))
			exit_error("empty strings aren't executable\n", NULL, NO, 127);
	ptr = get_path(array, env);
	if (!ptr)
		if (free_array(array))
			exit_error("%s wasn't found\n", av[child + 2], NO, 127);
	free(array[0]);
	array[0] = ptr;
	execve(array[0], array, env);
	free_array(array);
	exit_error("execve failed\n", NULL, NO, 126);
}

int	exit_error(char *msg, void *obj, int action, int code)
{
	if (action == OPEN_FAIL)
	{
		if (errno == EPERM || errno == EACCES)
			exit_error("%s lacks permissions\n", obj, NO, 1);
		else if (errno == ENOSPC)
			exit_error("%s lacks space\n", obj, NO, 0);
		else if (errno == ENOENT)
			exit_error("%s doesn't exist\n", obj, NO, 126);
		else if (errno == EISDIR)
			exit_error("%s is a directory\n", obj, NO, 126);
		else
			exit_error("undefined error with %s\n", obj, NO, 1);
	}
	if (code >= 0 && msg)
		ft_fprintf(2, "%d: ", code);
	if (ft_strnstr(msg, "%s", ft_strlen(msg)) != NULL)
		ft_fprintf(2, msg, (char *)obj);
	else if (msg)
		ft_fprintf(2, msg);
	if (action == FREE_OBJ)
		free(obj);
	if (code >= 0)
		exit(code);
	return (1);
}
