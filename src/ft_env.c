/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 09:41:59 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/26 12:35:39 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	ft_env(t_lstcmds *cmds, t_cmd *cmd, t_shell *sh)
{
	int	i;

	(void)cmd;
	if (!cmds->env)
		return (sh->exit_code = 1, 1);
	i = 0;
	while (cmds->env[i])
	{
		ft_putendl_fd(cmds->env[i], STDOUT_FILENO);
		i++;
	}
	sh->exit_code = 0;
	return (0);
}
