/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_end_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 15:10:06 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/29 10:50:54 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/quoicoushell.h"

int	print_perror(char *msg1, char *msg2)
{
	char	*buf;

	buf = ft_strjoin("quoicoushell: ", msg1);
	if (msg2)
	{
		buf = ft_strjoin_free(buf, ": ", 1, 0);
		buf = ft_strjoin_free(buf, msg2, 1, 0);
	}
	perror(buf);
	free(buf);
	return (1);
}

int	print_error(char *msg1, char *msg2)
{
	char	*buf;

	buf = ft_strjoin("quoicoushell: ", msg1);
	if (msg2)
	{
		buf = ft_strjoin_free(buf, ": ", 1, 0);
		buf = ft_strjoin_free(buf, msg2, 1, 0);
	}
	ft_putendl_fd(buf, STDERR_FILENO);
	free(buf);
	return (1);
}

int	stop_perror(char *msg, int error, t_lstcmds *cmds)
{
	int		i;

	if (error != 0)
		errno = error;
	else
		error = EXIT_FAILURE;
	print_perror(msg, NULL);
	i = -1;
	while (++i < 2)
	{
		if (cmds->fd[i][0] != -1)
			ft_close(cmds, cmds->fd[i][0]);
		if (cmds->fd[i][1] != -1)
			ft_close(cmds, cmds->fd[i][1]);
	}
	//free all remaining ptrs ?
	exit(EXIT_FAILURE);
}

int	stop_error(char *msg, int error, t_lstcmds *cmds)
{
	int		i;

	if (error == 127)
		print_error(msg, ": command not found");
	else
		print_error(msg, NULL);
	i = -1;
	while (++i < 2)
	{
		if (cmds->fd[i][0] != -1)
			ft_close(cmds, cmds->fd[i][0]);
		if (cmds->fd[i][1] != -1)
			ft_close(cmds, cmds->fd[i][1]);
	}
	//free all remaining ptrs ?
	exit(EXIT_FAILURE);
}
