/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sighandlers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 00:17:32 by jsommet           #+#    #+#             */
/*   Updated: 2024/09/24 13:11:39 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

void	signal_handler_main(int signum)
{
	g_sig = signum;
	if (signum == SIGINT)
	{
		rl_on_new_line();
		rl_replace_line("", 1);
		write(STDOUT_FILENO, "\n", 1);
		rl_redisplay();
	}
}

void	signal_handler_heredoc(int signum)
{
	g_sig = signum;
	if (signum == SIGINT)
	{
		ft_dprintf(2, "sighandler heredoc\n");
		rl_on_new_line();
		rl_replace_line("", 1);
		write(STDOUT_FILENO, "\n", 1);
		exit(EXIT_FAILURE);
	}
}

void	signal_handler_other(int signum)
{
	g_sig = signum;
	if (signum == SIGQUIT)
	{
		g_sig = SIGQUIT;
	}
}
