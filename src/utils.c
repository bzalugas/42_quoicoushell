/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 19:42:56 by bazaluga          #+#    #+#             */
/*   Updated: 2024/09/17 19:58:33 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

char	*readline_fd(t_shell *sh)
{
	int		tty_fd;
	int		stdout_fd;
	char	*line;
	int		is_tty;

	is_tty = 1;
	if (!isatty(STDOUT_FILENO))
		is_tty = 0;
	if (!is_tty)
	{
		tty_fd = open("/dev/tty", O_WRONLY);
		if (tty_fd == -1)
			exit_shell(sh, EXIT_FAILURE, false);
		stdout_fd = dup(STDOUT_FILENO);
		dup2(tty_fd, STDOUT_FILENO);
	}
	line = readline(sh->prompt);
	if (!is_tty)
	{
		dup2(stdout_fd, STDOUT_FILENO);
		close(tty_fd);
		close(stdout_fd);
	}
	return (line);
}
