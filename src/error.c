/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloimusa <aloimusa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:53:04 by aloimusa          #+#    #+#             */
/*   Updated: 2025/08/07 17:53:06 by aloimusa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	exec_cases(char *bin, char **env)
{
	char	*ptr;

	if (access(bin, X_OK) == 0)
		return (1);
	ptr = ft_strnstr(*env, "PATH=", 5);
	while (!ptr && *++env)
		ptr = ft_strnstr(*env, "PATH=", 5);
	if (ft_strncmp(bin, "./", 2) == 0 || !ptr)
	{
		if (errno == EPERM)
			ft_error("you don't have permissions for %s\n", bin, 1, 127);
		if (errno == EACCES)
			ft_error("%s: access fail\n", bin, 1, 126);
		else if (errno == ENOEXEC)
			ft_error("%s isn't executable\n", bin, 1, 127);
		else
			ft_error("%s can't be found\n", bin, 1, 127);
	}
	return (0);
}

void	ft_error(char *msg, char *obj, int do_free, int do_exit)
{
	if (obj && msg)
		ft_fprintf(2, msg, obj);
	else if (msg)
		ft_fprintf(2, msg);
	if (do_free == 1)
		free(obj);
	if (do_exit >= 0)
		exit(do_exit);
}
