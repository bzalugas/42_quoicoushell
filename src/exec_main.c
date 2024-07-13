/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 12:38:03 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/13 16:39:18 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/quoicoushell.h"
#include <stdlib.h>

static int	run_cmd(t_lstcmds *cmds, t_cmd *cmd)
{
	size_t	i;
	char	*abs_cmd;
	int		fd_in;
	int		fd_out;

	fd_in = cmds->fd[cmd->n_cmd % 2][0];
	fd_out = cmds->fd[(cmd->n_cmd + 1) % 2][1];
	if (fd_in > -1)
		dup2(fd_in, STDIN_FILENO);
	if (fd_out > -1)
		dup2(fd_out, STDOUT_FILENO);
	ft_close(cmds, fd_in);
	ft_close(cmds, fd_out);
	if (ft_strchr(cmd->cmd_opts[0], '/'))
		if (execve(cmd->cmd_opts[0], cmd->cmd_opts, cmds->env))
			stop_perror(cmd->cmd_opts[0], 0, cmds);
	i = 0;
	while (cmds->paths[i])
	{
		abs_cmd = ft_strjoin(cmds->paths[i], cmd->cmd_opts[0]);
		execve(abs_cmd, cmd->cmd_opts, cmds->env);
		free(abs_cmd);
		i++;
	}
	return (stop_error(cmd->cmd_opts[0], 127, cmds));
}

static int	prepare_run_cmd(t_lstcmds *cmds, t_cmd *cmd)
{
	pid_t	pid;
	int		pipe_in;
	int		pipe_out;

	pipe_in = cmd->n_cmd % 2;
	pipe_out = (cmd->n_cmd + 1) % 2;
	pid = fork();
	if (pid == -1)
		exit(errno);
	if (pid == 0)
	{
		ft_close(cmds, cmds->fd[pipe_in][1]);
		ft_close(cmds, cmds->fd[pipe_out][0]);
		if (cmd->redir_in)
			get_infile(cmds, cmd);
		if (cmd->redir_out)
			get_outfile(cmds, cmd);
		return (run_cmd(cmds, cmd));
	}
	ft_close(cmds, cmds->fd[pipe_in][0]);
	ft_close(cmds, cmds->fd[pipe_in][1]);
	return (pid);
}

int	run_all_cmds(t_lstcmds *cmds)
{
	t_list	*node_cmd;
	t_cmd	*cmd;
	pid_t	last;
	int		status;

	node_cmd = cmds->cmds;
	while (node_cmd && node_cmd->content)
	{
		cmd = node_cmd->content;
		if (cmd->lim_heredoc)
			get_heredoc(cmds, cmd);
		if (!cmd->redir_out && cmd->n_cmd < cmds->n_cmds - 1)
			if (pipe(cmds->fd[(cmd->n_cmd + 1) % 2]) == -1)
				exit(errno);
		last = prepare_run_cmd(cmds, cmd);
		node_cmd = node_cmd->next;
	}
	waitpid(last, &status, 0);
	while (errno != ECHILD)
		wait(NULL);
	return (WEXITSTATUS(status));
}
