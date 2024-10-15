/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_end_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 15:10:06 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/15 19:33:45 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/quoicoushell.h"

int	print_perror(char *msg1, char *msg2)
{
	char	*buf;

	buf = ft_strjoin("quoicoushell: ", msg1);
	if (msg2)
	{
		buf = ft_strjoin_free(buf, ": ", 1, 0);
		buf = ft_strjoin_free(buf, msg2, 1, 0);
	}
	perror(buf);
	free(buf);
	return (1);
}

int	print_error(char *msg1, char *msg2)
{
	char	*buf;

	buf = ft_strjoin("quoicoushell: ", msg1);
	if (msg2)
	{
		buf = ft_strjoin_free(buf, ": ", 1, 0);
		buf = ft_strjoin_free(buf, msg2, 1, 0);
	}
	ft_putendl_fd(buf, STDERR_FILENO);
	free(buf);
	return (1);
}

int	stop_perror(char *msg, int error, t_lstcmds *cmds, t_shell *sh)
{
	if (error != 0)
		errno = error;
	else
		error = EXIT_FAILURE;
	print_perror(msg, NULL);
	ft_close_all(cmds);
	ft_lstclear(&sh->local_vars, (&free_variable));
	ft_lstclear(&sh->env_vars, (&free_variable));
	free(sh->cwd);
	free(sh->prompt);
	free_split(sh->env);
	free_split(sh->paths);
	free_cmds(cmds);
	ft_lstclear(&sh->hist, &free);
	free(sh->hist_file);
	exit(EXIT_FAILURE);
}

int	stop_error(char *msg, int error, t_lstcmds *cmds, t_shell *sh)
{
	if (error == 127)
		print_error(msg, "command not found");
	else
		print_error(msg, NULL);
	ft_close_all(cmds);
	ft_lstclear(&sh->local_vars, (&free_variable));
	ft_lstclear(&sh->env_vars, (&free_variable));
	free(sh->cwd);
	free(sh->prompt);
	free_split(sh->env);
	free_split(sh->paths);
	free_cmds(cmds);
	ft_lstclear(&sh->hist, &free);
	free(sh->hist_file);
	exit(error);
}
