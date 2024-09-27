/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 17:33:01 by bazaluga          #+#    #+#             */
/*   Updated: 2024/09/26 08:59:59 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

static char	*random_filename(t_lstcmds *cmds, t_shell *sh)
{
	int		fd;
	char	filename[56];///tmp/qcshell-
	size_t	i;
	int		offset;

	fd = open("/dev/random", O_RDONLY);
	if (fd == -1)
		stop_error("heredoc", 1, cmds, sh);
	ft_memcpy(filename, "/tmp/qcshell-", 13);
	ft_bzero(&filename[13], 43);
	read(fd, &filename[13], 42);
	i = 0;
	while (i < 42)
	{
		filename[13 + i] = ((unsigned char)filename[13 + i]) % 62;
		offset = '0';
		if (filename[13 + i] > 9)
			offset += 'A' - '9' - 1;
		if (filename[13 + i] > 35)
			offset += 'a' - 'Z' - 1;
		filename[13 + i] += offset;
		i++;
	}
	close(fd);
	return (ft_strdup(filename));
	/* return (ft_strjoin_free("/tmp/", ft_strdup(filename), 0, 1)); */
}

static int	get_heredoc(t_shell *sh, t_lstcmds *cmds, t_cmd *cmd, int i)
{
	char	*line;
	char	*line_expanded;

	while (1)
	{
		line = readline("> ");
		if (g_sig == SIGINT)
			return (clean_heredocs(cmds, cmd));
		if (!line)
			break ;
		if (!ft_strcmp(cmd->heredocs[i], line))
		{
			free(line);
			close(cmds->fd[cmd->n_cmd % 2][0]);
			return (0);
		}
		else
		{
			line_expanded = expand_fhd(sh, line);
			ft_dprintf(cmds->fd[cmd->n_cmd % 2][0], "%s\n", line_expanded);
		}
		free(line);
	}
	return (0);
}

static int	run_heredoc(t_shell *sh, t_lstcmds *cmds, t_cmd *cmd, int i)
{
	int	pid;

	sh->sa.sa_handler = &signal_handler_other;
	sigaction(SIGINT, &sh->sa, &sh->sa_tmp);
	pid = fork();
	if (pid == -1)
		exit(errno);
	if (pid == 0)
	{
		sh->sa.sa_handler = &signal_handler_heredoc;
		sigaction(SIGINT, &sh->sa, &sh->sa_tmp);
		get_heredoc(sh, cmds, cmd, i);
		sigaction(SIGINT, &sh->sa_tmp, NULL);
		exit(EXIT_SUCCESS);
	}
	waitpid(pid, NULL, 0);
	sigaction(SIGINT, &sh->sa_tmp, NULL);
	return (0);
}

int	get_heredocs(t_lstcmds *cmds, t_cmd *cmd, t_shell *sh)
{
	int		i;

	if (!cmd->heredocs)
		return (1);
	i = -1;
	while (cmd->heredocs[++i])
	{
		if (i == 0)
			cmd->hd_filename = random_filename(cmds, sh);
		if (!cmd->hd_filename)
			stop_error("random filename", 1, cmds, sh);
		ft_close(cmds, cmds->fd[cmd->n_cmd % 2][0]);
		cmds->fd[cmd->n_cmd % 2][0] = open(cmd->hd_filename, O_WRONLY
				| O_CREAT | O_TRUNC, 0600);
		if (cmds->fd[cmd->n_cmd % 2][0] == -1)
			return (stop_error("in get heredocs", 1, cmds, sh));
		run_heredoc(sh, cmds, cmd, i);
	}
	if (cmd->heredoc)
	{
		ft_close(cmds, cmds->fd[cmd->n_cmd % 2][0]);
		cmds->fd[cmd->n_cmd % 2][0] = open(cmd->hd_filename, O_RDONLY);
	}
	return (0);
}
