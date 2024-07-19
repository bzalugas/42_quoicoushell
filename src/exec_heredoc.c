/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 17:33:01 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/19 21:40:34 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

static char	*random_filename(t_lstcmds *cmds)
{
	int		fd;
	char	buf[4096];
	char	filename[11];
	size_t	i;
	size_t	j;

	fd = open("/dev/random", O_RDONLY);
	if (fd == -1)
		stop_error("heredoc", 0, cmds);
	ft_bzero(buf, 4096);
	ft_bzero(filename, 11);
	j = 0;
	while (j < 10)
	{
		read(fd, buf, 4096);
		i = 0;
		while (i < 4096 && j < 10)
		{
			if (ft_isalnum(buf[i]))
				filename[j++] = buf[i];
			i++;
		}
	}
	close(fd);
	return (ft_strdup(filename));
}

int	get_heredoc(t_lstcmds *cmds, t_cmd *cmd, char *lim, char *filename)
{
	char	*line;

	//CHANGE THIS TO USE A TMP FILE (random name in /dev/random)
	 if (pipe(cmds->fd[cmd->n_cmd % 2]) == -1)
		exit(errno);
	lim = ft_strjoin(lim, "\n");
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

int	get_heredocs(t_lstcmds *cmds, t_cmd *cmd)
{
	int	i;

	if (!cmd->redirs)
		return (1);
	i = 0;
	while (cmd->heredocs[i])
	{
		get_heredoc(cmds, cmd, cmd->heredocs[i], random_filename(cmds));
		i++;
	}
	return (0);
}
