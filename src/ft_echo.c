/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 23:08:46 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/28 23:19:55 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	ft_echo(t_cmd *cmd)
{
	int		i;
	bool	nl;

	nl = true;
	if (cmd->argv[1] && !ft_strcmp(cmd->argv[1], "-n"))
		nl = false;
	i = 1 + (!nl);
	while (cmd->argv[i])
	{
		ft_putstr_fd(cmd->argv[i], STDOUT_FILENO);
		i++;
	}
	if (nl)
		write(1, "\n", STDOUT_FILENO);
	return (0);
}
