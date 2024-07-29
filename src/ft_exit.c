/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 22:14:35 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/30 00:20:32 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	ft_exit(t_cmd *cmd, t_shell *sh)
{
	unsigned int	status;
	size_t			i;

	if (!cmd->argv[1])
		exit_shell(sh, sh->exit_code, true);
	if (cmd->argc > 2)
		exit_shell(sh, 2, true);
	i = 0;
	while (cmd->argv[1][i])
	{
		if (!ft_isdigit(cmd->argv[1][i]))
			exit_shell(sh, 2, true);
		i++;
	}
	status = ft_atou_base(cmd->argv[1], "0123456789");
	exit_shell(sh, status % 256, true);
	return (0);
}
