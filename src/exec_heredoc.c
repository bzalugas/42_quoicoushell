/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 17:33:01 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/29 22:53:34 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

static char	*random_filename(t_lstcmds *cmds)
{
	int		fd;
	char	buf[4096];
	char	filename[43];
	size_t	i;
	size_t	j;

	fd = open("/dev/random", O_RDONLY);
	if (fd == -1)
		stop_error("heredoc", 1, cmds);
	ft_bzero(buf, 4096);
	ft_bzero(filename, 43);
	j = 0;
	while (j < 42)
	{
		read(fd, buf, 4096);
		i = 0;
		while (i < 4096 && j < 42)
		{
			if (j > 0 && j % 10 == 0)
				filename[j++] = '-';
			else if (ft_isalnum(buf[i]))
				filename[j++] = buf[i];
			i++;
		}
	}
	close(fd);
	return (ft_strjoin_free("/tmp/", ft_strdup(filename), 0, 1));
}

static int	clean_heredocs(t_lstcmds *cmds, t_cmd *cmd)
{
	ft_close(cmds, cmds->fd[cmd->n_cmd % 2][0]);
	unlink(cmd->hd_filename);
	cmd->hd_filename = NULL;
	return (2);
}

static int	get_heredoc(t_lstcmds *cmds, t_cmd *cmd, int i)
{
	char	*line;
	int		fdn;

	fdn = cmd->n_cmd % 2;
	ft_putstr_fd("> ", STDOUT_FILENO);
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (g_sigint == 1)
			return (clean_heredocs(cmds, cmd));
		if (!line)
			break ;
		if (!ft_strcmp(cmd->heredocs[i], line))
		{
			free(line);
			close(cmds->fd[0][0]);
			return (0);
		}
		else
			ft_putstr_fd(line, cmds->fd[fdn][0]);
		free(line);
		ft_putstr_fd("> ", STDOUT_FILENO);
	}
	return (0);
}

int	get_heredocs(t_lstcmds *cmds, t_cmd *cmd)
{
	int		i;

	if (!cmd->heredocs || !cmd->heredoc)
		return (1);
	i = 0;
	while (cmd->heredocs[i])
	{
		if (i == 0)
			cmd->hd_filename = random_filename(cmds);
		if (!cmd->hd_filename)
			stop_error("random filename", 1, cmds);
		ft_close(cmds, cmds->fd[cmd->n_cmd % 2][0]);
		cmds->fd[cmd->n_cmd % 2][0] = open(cmd->hd_filename, O_WRONLY | O_CREAT
			| O_TRUNC, 0600);
		if (cmds->fd[cmd->n_cmd % 2][0] == -1)
			return (stop_error("in get heredocs", 1, cmds));
		if (get_heredoc(cmds, cmd, i) == 2)
			return (2);
		i++;
	}
	ft_close(cmds, cmds->fd[cmd->n_cmd % 2][0]);
	cmds->fd[cmd->n_cmd % 2][0] = open(cmd->hd_filename, O_RDONLY);
	return (0);
}
