/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 09:41:59 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/28 22:42:47 by jsommet          ###   ########.fr       */
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
