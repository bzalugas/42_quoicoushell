/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:15:15 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/29 17:58:06 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

void	handle_exit_status(t_shell *sh, int status[2], pid_t last_pid)
{
	if (last_pid != -1 && WIFSIGNALED(status[0]))
	{
		g_sig = WTERMSIG(status[0]);
		if (WCOREDUMP(status[0]))
			ft_dprintf(STDERR_FILENO, " (core dumped)");
	}
	else if (last_pid != -1)
		sh->exit_code = WEXITSTATUS(status[0]);
	if (WIFSIGNALED(status[0]) || (WIFSIGNALED(status[1]) && !sh->cmds->has_hd))
		ft_putchar_fd('\n', STDOUT_FILENO);
}
