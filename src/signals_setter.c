/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_setter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:27:16 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/24 12:16:51 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

void	set_signals_main(t_shell *sh)
{
	sh->sa.sa_handler = &signal_handler_main;
	sigaction(SIGINT, &sh->sa, NULL);
	sh->sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sh->sa, NULL);
}

void	set_exec_parent_signals(t_shell *sh)
{
	/* sh->sa.sa_handler = &signal_handler_other; */
	sh->sa.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sh->sa, &sh->sa_tmp);
}

void	set_exec_child_signals(t_shell *sh)
{
	sh->sa.sa_handler = SIG_DFL;
	sigaction(SIGQUIT, &sh->sa, NULL); //=>try to add the (core dumped) text
	sh->sa.sa_handler = &signal_handler_child;
	sigaction(SIGINT, &sh->sa, NULL);//=>change it if not in a builtin to SIG_DFL
									 //(Maybe not necessary because no builtin will have
									 //the time to get C-c)
}
