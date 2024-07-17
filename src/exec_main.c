/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 12:38:03 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/17 19:44:38 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/quoicoushell.h"
#include <stdlib.h>

static int	run_non_builtin(t_lstcmds *cmds, t_cmd *cmd)
{
	size_t	i;
	char	*abs_cmd;

	if (ft_strchr(cmd->argv[0], '/'))
		if (execve(cmd->argv[0], cmd->argv, cmds->env))
			stop_perror(cmd->argv[0], 0, cmds);
	i = 0;
	while (cmds->paths[i])
	{
		abs_cmd = ft_strjoin(cmds->paths[i], cmd->argv[0]);
		execve(abs_cmd, cmd->argv, cmds->env);
		free(abs_cmd);
		i++;
	}
	return (stop_error(cmd->argv[0], 127, cmds));
}

static int	run_cmd(t_lstcmds *cmds, t_cmd *cmd)
{
	int		fd_in;
	int		fd_out;
	int		status;

	fd_in = cmds->fd[cmd->n_cmd % 2][0];
	fd_out = cmds->fd[(cmd->n_cmd + 1) % 2][1];
	if (fd_in > -1)
		dup2(fd_in, STDIN_FILENO);
	if (fd_out > -1)
		dup2(fd_out, STDOUT_FILENO);
	ft_close(cmds, fd_in);
	ft_close(cmds, fd_out);
	status = run_builtin(cmds, cmd, true);
	if (status)
		return (status);
	return (run_non_builtin(cmds, cmd));
}

static int	prepare_run_cmd(t_lstcmds *cmds, t_cmd *cmd, t_shell *sh)
{
	pid_t	pid;
	int		pipe_in;
	int		pipe_out;

	sh->exit_code = run_builtin(cmds, cmd, false);
	if (sh->exit_code)
		return (-1);
	pipe_in = cmd->n_cmd % 2;
	pipe_out = (cmd->n_cmd + 1) % 2;
	pid = fork();
	if (pid == -1)
		exit(errno);
	if (pid == 0)
	{
		ft_close(cmds, cmds->fd[pipe_in][1]);
		ft_close(cmds, cmds->fd[pipe_out][0]);

		return (run_cmd(cmds, cmd));
	}
	ft_close(cmds, cmds->fd[pipe_in][0]);
	ft_close(cmds, cmds->fd[pipe_in][1]);
	return (pid);
}

int	run_all_cmds(t_lstcmds *cmds, t_shell *sh)
{
	t_list	*node_cmd;
	t_cmd	*cmd;
	pid_t	last;

	cmds->env = export_env(sh);
	node_cmd = cmds->cmds;
	while (node_cmd && node_cmd->content)
	{
		cmd = node_cmd->content;
		if (cmd->lim_heredoc) //do a while
			get_heredoc(cmds, cmd);
		if (!cmd->redir_out && cmd->n_cmd < cmds->n_cmds - 1)
			if (pipe(cmds->fd[(cmd->n_cmd + 1) % 2]) == -1)
				exit(errno);
		last = prepare_run_cmd(cmds, cmd, sh);
		node_cmd = node_cmd->next;
	}
	if (last != -1)
		waitpid(last, &sh->exit_code, 0);
	while (errno != ECHILD)
		wait(NULL);
	if (last != -1)
		sh->exit_code = WEXITSTATUS(sh->exit_code);
	return (sh->exit_code);
}
