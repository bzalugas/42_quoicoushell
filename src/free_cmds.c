/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 10:08:29 by bazaluga          #+#    #+#             */
/*   Updated: 2024/09/27 11:54:19 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/quoicoushell.h"

void	free_cmd(void *content)
{
	t_cmd	*cmd;
	t_redir	*tmp;

	cmd = (t_cmd *)content;
	free_split(cmd->argv);
	tmp = cmd->redirs;
	while (tmp->file)
	{
		free(tmp->file);
		tmp++;
	}
	free(cmd->redirs);
	free_split(cmd->heredocs);
	free(cmd->hd_filename);
	free(cmd);
}

void	free_cmds(t_lstcmds *cmds)
{
	ft_lstclear(&cmds->cmds, free_cmd);
}
