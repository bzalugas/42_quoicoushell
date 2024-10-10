/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 17:33:01 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/10 12:55:09 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

static char	*random_filename(t_shell *sh, t_lstcmds *cmds)
{
	int		fd;
	char	filename[56];
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
}

static int	get_heredoc(t_shell *sh, t_lstcmds *cmds, t_cmd *cmd, int i)
{
	char	*line;
	char	*line_expanded;

	while (1)
	{
		line = readline("> ");
		if (g_sig == SIGINT)
			return (clean_heredocs(cmds, cmd, CLEAN_FORK));
		if (!line)
			return (eof_ending_heredoc(cmds, cmd, cmd->heredocs[i]));
		if (!ft_strcmp(cmd->heredocs[i], line))
		{
			free(line);
			return (clean_heredocs(cmds, cmd, CLEAN_FORK));
		}
		else
		{
			line_expanded = expand_fhd(sh, line);
			ft_dprintf(cmd->fd_hd[1], "%s\n", line_expanded);
			free(line_expanded);
		}
		free(line);
	}
	return (0);
}

static int	run_heredoc(t_shell *sh, t_lstcmds *cmds, t_cmd *cmd, int i)
{
	int	pid;
	int	status;

	status = 0;
	sh->sa.sa_handler = &signal_handler_other;
	sigaction(SIGINT, &sh->sa, &sh->sa_tmp);
	pid = fork();
	if (pid == -1)
		exit(errno);
	if (pid == 0)
	{
		sh->sa.sa_handler = &signal_handler_heredoc;
		sigaction(SIGINT, &sh->sa, NULL);
		exit_shell(sh, get_heredoc(sh, cmds, cmd, i), false);
	}
	waitpid(pid, &status, 0);
	sigaction(SIGINT, &sh->sa_tmp, NULL);
	if (g_sig == SIGINT)
		sh->exit_code = 130;
	else
		sh->exit_code = WEXITSTATUS(status);
	return (sh->exit_code);
}

static int	get_heredocs_of_cmd( t_shell *sh, t_lstcmds *cmds, t_cmd *cmd)
{
	int	i;

	if (!cmd->heredocs || !cmd->heredocs[0])
		return (0);
	i = -1;
	while (cmd->heredocs[++i])
	{
		if (i == 0)
			cmd->hd_file = random_filename(sh, cmds);
		if (!cmd->hd_file)
			stop_error("random filename", 1, cmds, sh);
		ft_close(cmds, cmd->fd_hd[1]);
		ft_close(cmds, cmd->fd_hd[0]);
		cmd->fd_hd[1] = open(cmd->hd_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
		cmd->fd_hd[0] = open(cmd->hd_file, O_RDONLY);
		unlink(cmd->hd_file);
		if (cmd->fd_hd[0] == -1 || cmd->fd_hd[1] == -1)
			return (stop_error("problem with here-document", 1, cmds, sh));
		if (run_heredoc(sh, cmds, cmd, i) != 0)
			return (clean_heredocs(cmds, cmd, CLEAN_MAIN));
	}
	ft_close(cmds, cmd->fd_hd[1]);
	free(cmd->hd_file);
	cmd->hd_file = NULL;
	return (0);
}

int	get_all_heredocs(t_shell *sh, t_lstcmds *cmds)
{
	t_list	*node_cmd;
	t_cmd	*cmd;

	node_cmd = cmds->cmds;
	while (node_cmd && node_cmd->content)
	{
		cmd = node_cmd->content;
		cmd->idx_in = cmd->n_cmd % 2;
		cmd->idx_out = (cmd->n_cmd + 1) % 2;
		if (get_heredocs_of_cmd(sh, cmds, cmd) != 0)
			return (1);
		node_cmd = node_cmd->next;
	}
	return (0);
}
