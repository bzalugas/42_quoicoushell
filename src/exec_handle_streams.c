/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle_streams.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 15:31:57 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/26 11:36:52 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	ft_close(t_lstcmds *cmds, int fd)
{
	/* if (fd == -1) */
	/* { */
	/* 	ft_putendl_fd("Trying to close -1", STDERR_FILENO); //only for debugging */
	/* 	exit(-1); // change to return once no debugging needed */
	/* } */
	if (fd == -1)
		return (-1);
	if (cmds->fd[0][0] == fd)
		cmds->fd[0][0] = -1;
	if (cmds->fd[0][1] == fd)
		cmds->fd[0][1] = -1;
	if (cmds->fd[1][0] == fd)
		cmds->fd[1][0] = -1;
	if (cmds->fd[1][1] == fd)
		cmds->fd[1][1] = -1;
	return (close(fd));
}

static int	get_infile(t_lstcmds *cmds, t_cmd *cmd, char *filename, bool forked)
{
	int	fdn;

	fdn = cmd->n_cmd % 2;
	ft_close(cmds, cmds->fd[fdn][0]);
	cmds->fd[fdn][0] = open(filename, O_RDONLY);
	if (cmds->fd[fdn][0] == -1)
	{
		if (forked)
			stop_perror(filename, 0, cmds);
		perror(filename);
	}
	return (0);
}

static int	get_outfile(t_lstcmds *cmds, t_cmd *cmd, int redir_i, bool forked)
{
	int		fdn;
	t_redir	redir;

	redir = cmd->redirs[redir_i];
	fdn = (cmd->n_cmd + 1) % 2;
	ft_close(cmds, cmds->fd[fdn][1]);
	if (redir.type == RTOUT_A)
		cmds->fd[fdn][1] = open(redir.file, O_WRONLY | O_CREAT | O_APPEND,
			0644);
	else
		cmds->fd[fdn][1] = open(redir.file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (cmds->fd[fdn][1] == -1)
	{
		if (forked)
			stop_perror(redir.file, 0, cmds);
		perror(redir.file);
	}
	return (0);
}

int	get_in_out_files(t_lstcmds *cmds, t_cmd *cmd, bool forked)
{
	int	i;
	if (!cmd->redirs)
		return (1);
	i = 0;
	while (cmd->redirs[i].file)
	{
		if (cmd->redirs[i].type == RTIN)
			get_infile(cmds, cmd, cmd->redirs[i].file, forked);
		else
			get_outfile(cmds, cmd, i, forked);
		i++;
	}
	return (0);
}
