/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 23:08:46 by bazaluga          #+#    #+#             */
/*   Updated: 2024/09/30 18:26:29 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	ft_echo(t_cmd *cmd)
{
	int		i;
	bool	nl;

	nl = !(cmd->argv[1] && !ft_strcmp(cmd->argv[1], "-n"));
	i = 1 + (!nl);
	while (cmd->argv[i])
	{
		ft_putstr_fd(cmd->argv[i], STDOUT_FILENO);
		if (cmd->argv[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (nl)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
