/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:15:15 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/30 18:17:29 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

void	handle_exit_status(t_shell *sh, pid_t lastpid, int status, bool sig)
{
	if (lastpid != -1 && WIFSIGNALED(status))
	{
		g_sig = WTERMSIG(status);
		if (WCOREDUMP(status))
			ft_dprintf(STDERR_FILENO, " (core dumped)");
	}
	else if (lastpid != -1)
		sh->exit_code = WEXITSTATUS(status);
	if (WIFSIGNALED(status) || sig)
		ft_putchar_fd('\n', STDOUT_FILENO);
}

bool	wait_all(pid_t last_pid, int *last_status)
{
	int		status;
	bool	signal;

	status = 0;
	signal = false;
	if (last_pid != -1)
		waitpid(last_pid, last_status, 0);
	while (errno != ECHILD)
	{
		wait(&status);
		if (errno != ECHILD && WIFSIGNALED(status)
			&& (WTERMSIG(status) == SIGINT || WTERMSIG(status) == SIGQUIT))
			signal = true;
	}
	return (signal);
}
