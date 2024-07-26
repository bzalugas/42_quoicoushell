/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 06:24:12 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/26 09:58:32 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	run_builtin(t_lstcmds *cmds, t_cmd *cmd, t_shell *sh, bool forked)
{
	//Handle redirs & heredocs etc
	(void)cmds;
	(void)sh;
	if (cmd->n_cmd > 0 && !forked)
		return (0);
	if (!ft_strcmp(cmd->argv[0], "echo"))
		return (1);
	if (!ft_strcmp(cmd->argv[0], "cd"))
		return (1);
	if (!ft_strcmp(cmd->argv[0], "pwd"))
		return (1);
	if (!ft_strcmp(cmd->argv[0], "export"))
		return (ft_export(cmds, cmd, sh));
	if (!ft_strcmp(cmd->argv[0], "unset"))
		return (1);
	if (!ft_strcmp(cmd->argv[0], "env"))
		return (ft_env(cmds, cmd, sh));
	if (!ft_strcmp(cmd->argv[0], "exit"))
		return (1);
	//handle a=34
	return (0);
}
