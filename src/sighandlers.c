/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sighandlers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 00:17:32 by jsommet           #+#    #+#             */
/*   Updated: 2024/10/24 12:14:36 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

void	signal_handler_main(int signum)
{
	g_sig = signum;
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
		init_shell(NULL)->exit_code = 130;
	}
}

void	signal_handler_heredoc(int signum)
{
	g_sig = signum;
	if (signum == SIGINT)
	{
		/* write(STDOUT_FILENO, "\n", 1); */
		/* rl_on_new_line(); */
		/* rl_replace_line("", 1); *///=>to do in the readline_fd fun or to tty_fd if exiting
									 //Maybe cleaner to exit_shell(init_shell(NULL))
		close(STDIN_FILENO);
	}
}

void	signal_handler_other(int signum)
{
	g_sig = signum;
	/* if (signum == SIGINT) */
	/* 	init_shell(NULL)->exit_code = 130; */
	/* exit(130); */
}

void	signal_handler_child(int signum)
{
	g_sig = signum;
	if (signum == SIGINT)
		exit(130);
}
