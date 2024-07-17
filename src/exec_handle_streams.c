/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle_streams.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 15:31:57 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/17 20:57:02 by bazaluga         ###   ########.fr       */
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

	//CHANGE THIS TO USE A TMP FILE (random name in /dev/random)
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

static int	get_infile(t_lstcmds *cmds, t_cmd *cmd, char *filename)
{
	int	fdn;

	fdn = cmd->n_cmd % 2;
	if (cmds->fd[fdn][0] != -1)
		ft_close(cmds, cmds->fd[fdn][0]);
	cmds->fd[fdn][0] = open(filename, O_RDONLY);
	if (cmds->fd[fdn][0] == -1)
		stop_perror(filename, 0, cmds);
	return (0);
}

static int	get_outfile(t_lstcmds *cmds, t_cmd *cmd, int redir_i)
{
	int		fdn;
	t_redir	redir;

	redir = cmd->redirs[redir_i];
	fdn = (cmd->n_cmd - 1) % 2;
	if (cmds->fd[fdn][1] != -1)
		ft_close(cmds, cmds->fd[fdn][1]);
	if (redir.type == RTOUT_A)
		cmds->fd[fdn][1] = open(redir.file, O_WRONLY | O_CREAT | O_APPEND,
			0644);
	else
		cmds->fd[fdn][1] = open(redir.file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (cmds->fd[fdn][1] == -1)
		stop_perror(redir.file, 0, cmds);
	return (0);
}

int	get_in_out_files(t_lstcmds *cmds, t_cmd *cmd)
{
	int	i;
//SEPARATE HEREDOC (in main process) and in/out files (child process)
	if (!cmd->redirs)
		return (1);
	i = 0;
	while (cmd->redirs[i].file)
	{
		if (cmd->redirs[i].type == RTIN)
			get_infile(cmds, cmd, cmd->redirs[i].file);
		else if (cmd->redirs[i].type == RTHEREDOC)
			get_heredoc(cmds, cmd);
		else
			get_outfile(cmds, cmd, i);
		i++;
	}
}
