/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 23:08:46 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/10 11:37:15 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	ft_echo(t_shell *sh, t_cmd *cmd)
{
	int		i;
	bool	nl;

	sh->exit_code = 0;
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
	return (sh->exit_code);
}
