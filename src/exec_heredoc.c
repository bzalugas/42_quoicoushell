/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 17:33:01 by bazaluga          #+#    #+#             */
/*   Updated: 2024/09/17 10:49:35 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

static char	*random_filename(t_lstcmds *cmds, t_shell *sh)
{
	int		fd;
	char	filename[43];
	size_t	i;
	int		offset;

	fd = open("/dev/random", O_RDONLY);
	if (fd == -1)
		stop_error("heredoc", 1, cmds, sh);
	ft_bzero(filename, 43);
	read(fd, filename, 42);
	i = 0;
	while (i < 42)
	{
		filename[i] = ((unsigned char)filename[i]) % 62;
		offset = '0';
		if (filename[i] > 9)
			offset += 'A' - '9' - 1;
		if (filename[i] > 35)
			offset += 'a' - 'Z' - 1;
		filename[i] += offset;
		i++;
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

static int	get_heredoc(t_shell *sh, t_lstcmds *cmds, t_cmd *cmd, int i)
{
	char	*line;
	char	*line_expanded;

	while (1)
	{
		line = readline("> ");
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
			line_expanded = expand_fhd(sh, line);
		free(line);
	}
	return (0);
}

int	get_heredocs(t_lstcmds *cmds, t_cmd *cmd, t_shell *sh)
{
	int		i;

	if (!cmd->heredocs)
		return (1);
	i = 0;
	while (cmd->heredocs[i])
	{
		if (i == 0)
			cmd->hd_filename = random_filename(cmds, sh);
		if (!cmd->hd_filename)
			stop_error("random filename", 1, cmds, sh);
		ft_close(cmds, cmds->fd[cmd->n_cmd % 2][0]);
		cmds->fd[cmd->n_cmd % 2][0] = open(cmd->hd_filename, O_WRONLY | O_CREAT
			| O_TRUNC, 0600);
		if (cmds->fd[cmd->n_cmd % 2][0] == -1)
			return (stop_error("in get heredocs", 1, cmds, sh));
		if (get_heredoc(sh, cmds, cmd, i) == 2)
			return (2);
		i++;
	}
	if (cmd->heredoc)
	{
		ft_close(cmds, cmds->fd[cmd->n_cmd % 2][0]);
		cmds->fd[cmd->n_cmd % 2][0] = open(cmd->hd_filename, O_RDONLY);
	}
	return (0);
}
