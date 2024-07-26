/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:18:30 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/28 22:43:12 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	ft_unset(t_cmd *cmd, t_shell *sh)
{
	int	i;

	i = 1;
	while (cmd->argv[i])
	{
		del_variable(sh, cmd->argv[i]);
		i++;
	}
	if (i > 1)
		sh->env_update = true;
	sh->exit_code = 0;
	return (0);
}
