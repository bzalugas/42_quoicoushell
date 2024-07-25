/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 10:08:29 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/17 14:33:23 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/quoicoushell.h"

void	free_cmd(void *content)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)content;
	free_split(cmd->argv);
	free(cmd->redir_in);
	free(cmd->redir_out);
	free(cmd->lim_heredoc);
	free(cmd);
}

void	free_cmds(t_lstcmds *cmds)
{
	ft_lstclear(&cmds->cmds, free_cmd);
	free_split(cmds->paths);
}
