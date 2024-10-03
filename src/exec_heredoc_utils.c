/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 13:05:54 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/03 13:07:36 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	clean_heredocs(t_lstcmds *cmds, t_cmd *cmd, int clean_case)
{
	/* ft_close(cmds, cmds->fd[cmd->n_cmd % 2][0]); */
	ft_close(cmds, cmd->fd_hd[1]);
	/* if (cmd->fd_hd[1] > -1 && close(cmd->fd_hd[1]) == 0) */
	/* 	cmd->fd_hd[1] = -1; */
	if (g_sig != 0 && clean_case == CLEAN_FORK)
		ft_close(cmds, cmd->fd_hd[0]);
		/* if (cmd->fd_hd[0] > -1 && close(cmd->fd_hd[0]) == 0) */
			/* cmd->fd_hd[0] = -1; */
	/* if (g_sig != 0 && clean_case == CLEAN_FORK) */
	/* 	unlink(cmd->hd_file); */
	/* free(cmd->hd_file); */
	/* cmd->hd_file = NULL; */

	return (clean_case + (g_sig != 0));
}

int	eof_ending_heredoc(t_lstcmds *cmds, t_cmd *cmd, char *delim)
{
	ft_dprintf(STDERR_FILENO,
		"quoicoushell: warning: here-document delimited by end-of-file\
 (wanted `%s')\n", delim);
	return (clean_heredocs(cmds, cmd, CLEAN_FORK));
}
