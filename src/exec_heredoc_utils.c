/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 13:05:54 by bazaluga          #+#    #+#             */
/*   Updated: 2024/09/27 12:33:23 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	clean_heredocs(t_lstcmds *cmds, t_cmd *cmd, int clean_case)
{
	ft_close(cmds, cmds->fd[cmd->n_cmd % 2][0]);
	if (g_sig != 0 && clean_case == CLEAN_FORK)
		unlink(cmd->hd_filename);
	free(cmd->hd_filename);
	cmd->hd_filename = NULL;
	return (clean_case + (g_sig != 0));
}

int	eof_ending_heredoc(t_lstcmds *cmds, t_cmd *cmd, char *delim)
{
	ft_dprintf(STDERR_FILENO,
		"quoicoushell: warning: here-document delimited by end-of-file\
 (wanted `%s')\n", delim);
	return (clean_heredocs(cmds, cmd, CLEAN_FORK));
}
