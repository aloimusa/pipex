/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloimusa <aloimusa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 16:49:05 by aloimusa          #+#    #+#             */
/*   Updated: 2025/06/19 16:49:16 by aloimusa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>

enum		e_pipe_head
{
	IN = 0,
	OUT = 1,
	READ = 0,
	WRITE = 1,
};

enum		e_pipe_order
{
	IOFD = 0,
	LAST = 1,
	NEXT = 2,
};

typedef struct s_pipex
{
	int		cmdc;
	char	***cmdv;
	int		fd[3][2];
	pid_t	*pid;
	int		c_index;
}			t_pipex;

typedef struct s_pipe
{
	int		child;
	int		fd[3][2];
	int		*pid;
}			t_pipe;

int			exec_cases(char *bin, char **env);
void		ft_error(char *msg, char *obj, int do_free, int do_exit);
void		init_pipes(int ac, char **av, t_pipe *p);
void		execute(char **av, char **env, int child);
void		cleanup(int ac, t_pipe *p);
int			code(void *str);
void		check_files(char **av, int ac);
char		*str_to_bin(char *bin, char **env);
int			free_pipex(char **array, t_pipe *p);
#endif
