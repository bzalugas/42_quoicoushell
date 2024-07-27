/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 10:08:29 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/27 13:20:51 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/quoicoushell.h"

void	free_cmd(void *content)
{
	t_cmd	*cmd;
	t_redir *tmp;

	cmd = (t_cmd *)content;
	tmp = cmd->redirs;
	while (cmd->redirs->file)
	{
		free(cmd->redirs->file);
		cmd->redirs++;
	}
	free(tmp);
	free_split(cmd->heredocs);
	free_split(cmd->argv);
	free(cmd);
}

void	free_cmds(t_lstcmds *cmds)
{
	ft_lstclear(&cmds->cmds, free_cmd);
}
