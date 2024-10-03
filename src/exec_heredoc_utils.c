/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 13:05:54 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/03 14:21:16 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	clean_heredocs(t_lstcmds *cmds, t_cmd *cmd, int clean_case)
{
	ft_close(cmds, cmd->fd_hd[1]);
	if (g_sig != 0 && clean_case == CLEAN_FORK)
		ft_close(cmds, cmd->fd_hd[0]);
	return (clean_case + (g_sig != 0));
}

int	eof_ending_heredoc(t_lstcmds *cmds, t_cmd *cmd, char *delim)
{
	ft_dprintf(STDERR_FILENO,
		"quoicoushell: warning: here-document delimited by end-of-file\
 (wanted `%s')\n", delim);
	return (clean_heredocs(cmds, cmd, CLEAN_FORK));
}
