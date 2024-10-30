/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sighandlers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 00:17:32 by jsommet           #+#    #+#             */
/*   Updated: 2024/10/30 16:45:54 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

void	signal_handler_simple(int signum)
{
	g_sig = signum;
}

void	signal_handler_child(int signum)
{
	g_sig = signum;
	exit(128 + signum);
}

int	readline_check_signal(void)
{
	if (g_sig != 0)
	{
		rl_replace_line("", 0);
		rl_done = 1;
		return (1);
	}
	return (0);
}
