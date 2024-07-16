/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 06:24:12 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/16 19:37:19 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/quoicoushell.h"

int	run_builtin(t_lstcmds *cmds, t_cmd *cmd)
{
	(void)cmds;
	if (!ft_strcmp(cmd->cmd_opts[0], "echo"))
		return (1);
	if (!ft_strcmp(cmd->cmd_opts[0], "cd"))
		return (1);
	if (!ft_strcmp(cmd->cmd_opts[0], "pwd"))
		return (1);
	if (!ft_strcmp(cmd->cmd_opts[0], "export"))
		return (1);
	if (!ft_strcmp(cmd->cmd_opts[0], "unset"))
		return (1);
	if (!ft_strcmp(cmd->cmd_opts[0], "env"))
		return (1);
	if (!ft_strcmp(cmd->cmd_opts[0], "exit"))
		return (1);
	return (0);
}
