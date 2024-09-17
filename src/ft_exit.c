/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 22:14:35 by bazaluga          #+#    #+#             */
/*   Updated: 2024/09/17 19:09:46 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	ft_exit(t_cmd *cmd, t_shell *sh)
{
	size_t			i;

	if (!cmd->argv[1])
		exit_shell(sh, sh->exit_code, true);
	if (cmd->argc > 2)
	{
		ft_dprintf(STDERR_FILENO,
			"exit\nminishell: exit: too many arguments\n");
		return (1);
	}
	i = 0;
	while (cmd->argv[1][i])
	{
		if (!ft_isdigit(cmd->argv[1][i]))
		{
			ft_dprintf(STDERR_FILENO,
				"exit\nminishell: exit: %s: numeric argument required\n",
				cmd->argv[1]);
			exit_shell(sh, 2, false);
		}
		i++;
	}
	exit_shell(sh, ft_atou_base(cmd->argv[1], "0123456789") % 256, true);
	return (0);
}
