/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle_streams2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:45:54 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/03 14:48:02 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	ft_close_all_heredocs(t_lstcmds *cmds, t_cmd *except_cmd)
{
	t_list	*node;

	node = cmds->cmds;
	while (node && node->content)
	{
		if (node->content != except_cmd)
		{
			ft_close(cmds, ((t_cmd *)(node->content))->fd_hd[0]);
			ft_close(cmds, ((t_cmd *)(node->content))->fd_hd[1]);
		}
		node = node->next;
	}
	return (0);
}
