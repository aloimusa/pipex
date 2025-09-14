/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloimusa <aloimusa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 07:42:10 by aloimusa          #+#    #+#             */
/*   Updated: 2025/09/12 07:47:08 by aloimusa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdlib.h>
# include <sys/wait.h>

enum	e_pipe_head
{
	IN = 0,
	OUT = 1,
	READ = 0,
	WRITE = 1,
};

enum	e_pipe_order
{
	IOFD = 0,
	LAST = 1,
	NEXT = 2,
};

enum	e_error_action
{
	NO = -1,
	PRINT_OBJ,
	FREE_OBJ,
	NULL_OBJ,
	OPEN_FAIL,
};

typedef struct s_pipe
{
	int	child;
	int	fd[3][2];
	int	*pid;
}		t_pipe;

int		exit_error(char *msg, void *obj, int action, int code);
void	open_fail(char *file);
char	*get_path(char **prog, char **env);
int		free_array(char **array);
void	child(int ac, char **av, char **env, t_pipe p);
#endif
