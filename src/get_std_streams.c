/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_std_streams.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 12:38:16 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/07 12:51:12 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"
//don't forget to close all fds in exit_shell etc
int	get_stdin(void)
{
	static int	fd_stdin = -1;

	if (fd_stdin == -1)
		fd_stdin = dup(STDIN_FILENO);
	return fd_stdin;
}

int	get_stdout(void)
{
	static int	fd_stdout = -1;

	if (fd_stdout == -1)
		fd_stdout = dup(STDOUT_FILENO);
	return fd_stdout;
}

int	get_tty(void)
{
	static int	fd_tty = -1;

	if (fd_tty == -1)
		fd_tty = open(ttyname(STDIN_FILENO), O_WRONLY);
	return (fd_tty);
}
