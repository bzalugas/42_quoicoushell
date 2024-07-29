/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 09:41:59 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/27 13:22:38 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	ft_env(t_shell *sh)
{
	int	i;

	if (!sh->env)
		return (sh->exit_code = 1, 1);
	i = 0;
	while (sh->env[i])
	{
		ft_putendl_fd(sh->env[i], STDOUT_FILENO);
		i++;
	}
	sh->exit_code = 0;
	return (0);
}
