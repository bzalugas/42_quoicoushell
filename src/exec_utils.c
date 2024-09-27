/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 12:16:21 by bazaluga          #+#    #+#             */
/*   Updated: 2024/09/27 12:18:52 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

void	set_exec_parent_signals(t_shell *sh)
{
	sh->sa.sa_handler = &signal_handler_other;
	sigaction(SIGINT, &sh->sa, &sh->sa_tmp);
}

void	set_exec_child_signals(t_shell *sh)
{
	sh->sa.sa_handler = SIG_DFL;
	sigaction(SIGQUIT, &sh->sa, NULL);
}
