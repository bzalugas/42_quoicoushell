/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle_streams.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 15:31:57 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/13 16:18:23 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/quoicoushell.h"

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

int	get_heredoc(t_lstcmds *cmds, t_cmd *cmd)
{
	char	*line;
	char	*lim;

	if (pipe(cmds->fd[cmd->n_cmd % 2]) == -1)
		exit(errno);
	lim = ft_strjoin(cmd->lim_heredoc, "\n");
	ft_putstr_fd("> ", STDOUT_FILENO);
	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (!ft_strcmp(lim, line))
		{
			free(line);
			free(lim);
			close(cmds->fd[0][1]);
			return (0);
		}
		else
			ft_putstr_fd(line, cmds->fd[0][1]);
		free(line);
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
	}
	return (0);
}

int	get_infile(t_lstcmds *cmds, t_cmd *cmd)
{
	cmds->fd[cmd->n_cmd % 2][0] = open(cmd->redir_in, O_RDONLY);
	if (cmds->fd[cmd->n_cmd % 2][0] == -1)
		stop_perror(cmd->redir_in, 0, cmds);
	return (0);
}

int	get_outfile(t_lstcmds *cmds, t_cmd *cmd)
{
	int	fdn;

	fdn = (cmd->n_cmd - 1) % 2;
	if (cmd->out_append)
		cmds->fd[fdn][1] = open(cmd->redir_out,
			O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		cmds->fd[fdn][1] = open(cmd->redir_out,
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (cmds->fd[fdn][1] == -1)
		stop_perror(cmd->redir_out, 0, cmds);
	return (0);
}
