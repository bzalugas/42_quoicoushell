/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 12:38:03 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/30 17:53:36 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

static int	run_non_builtin(t_lstcmds *cmds, t_cmd *cmd, t_shell *sh)
{
	ssize_t	i;
	char	*abs_cmd;
	int		err;

	err = 127;
	set_execve_signals(sh);
	if (!cmd->argv[0])
		exit_shell(sh, EXIT_SUCCESS, false);
	if (ft_strchr(cmd->argv[0], '/'))
		if (execve(cmd->argv[0], cmd->argv, sh->env))
			stop_perror(cmd->argv[0], (126 * (errno == EACCES))
				+ (127 * (errno == ENOENT)), cmds, sh);
	i = -1;
	while (sh->paths && sh->paths[++i])
	{
		abs_cmd = ft_strjoin(sh->paths[i], cmd->argv[0]);
		execve(abs_cmd, cmd->argv, sh->env);
		if (err == 127)
			err = (126 * (errno == EACCES)) + (127 * (errno == ENOENT));
		free(abs_cmd);
	}
	if (err == 127)
		return (stop_error(cmd->argv[0], err, cmds, sh));
	return (stop_perror(cmd->argv[0], err, cmds, sh));
}

static int	run_cmd(t_lstcmds *cmds, t_cmd *cmd, t_shell *sh)
{
	int		fd_in;
	int		fd_out;
	int		status;

	fd_in = cmds->fd[cmd->idx_in][0];
	fd_out = cmds->fd[cmd->idx_out][1];
	if (fd_in > -1)
		dup2(fd_in, STDIN_FILENO);
	if (fd_out > -1)
		dup2(fd_out, STDOUT_FILENO);
	ft_close(cmds, fd_in);
	ft_close(cmds, fd_out);
	status = run_builtin(cmds, cmd, sh, true);
	if (status)
		exit_shell(sh, sh->exit_code, false);
	return (run_non_builtin(cmds, cmd, sh));
}

static int	prepare_run_cmd(t_lstcmds *cmds, t_cmd *cmd, t_shell *sh)
{
	pid_t	pid;

	if (cmds->n_cmds == 1 && run_builtin(cmds, cmd, sh, false) == 1)
		return (-1);
	pid = fork();
	if (pid == -1)
		stop_main_error(sh, "fork failed", errno);
	if (pid == 0)
	{
		set_exec_child_signals(sh);
		ft_close(cmds, cmds->fd[cmd->idx_in][1]);
		ft_close(cmds, cmds->fd[cmd->idx_out][0]);
		ft_close_all_heredocs(cmds, cmd);
		if (cmd->fd_hd[0] != -1)
		{
			ft_close(cmds, cmds->fd[cmd->idx_in][0]);
			cmds->fd[cmd->idx_in][0] = cmd->fd_hd[0];
		}
		if (get_in_out_files(sh, cmd) == -1)
			exit_shell(sh, EXIT_FAILURE, false);
		return (run_cmd(cmds, cmd, sh));
	}
	ft_close(cmds, cmds->fd[cmd->idx_in][0]);
	ft_close(cmds, cmds->fd[cmd->idx_in][1]);
	return (pid);
}

static int	iterate_cmds(t_lstcmds *cmds, t_shell *sh)
{
	t_list	*node_cmd;
	t_cmd	*cmd;
	pid_t	last;

	last = -1;
	set_exec_parent_signals(sh);
	if (get_all_heredocs(sh, cmds) != 0)
		return (last);
	node_cmd = cmds->cmds;
	while (node_cmd && node_cmd->content)
	{
		if (g_sig == SIGINT)
			return (last);
		g_sig = 0;
		cmd = node_cmd->content;
		if (cmd->n_cmd < cmds->n_cmds - 1)
			if (pipe(cmds->fd[(cmd->n_cmd + 1) % 2]) == -1)
				stop_main_error(sh, "pipe failed", errno);
		last = prepare_run_cmd(cmds, cmd, sh);
		node_cmd = node_cmd->next;
	}
	return (last);
}

int	run_all_cmds(t_lstcmds *cmds, t_shell *sh)
{
	pid_t	last;
	int		last_status;
	bool	signaled;

	last_status = -1;
	if (cmds->n_cmds == 1 && (!((t_cmd *)(cmds->cmds->content))->argv[0])
		&& !(((t_cmd *)(cmds->cmds->content))->heredoc)
		&& !(((t_cmd *)(cmds->cmds->content))->redirs[0].file))
		return (sh->exit_code);
	if (sh->env_update)
	{
		sh->env_update = false;
		free_split(sh->env);
		free_split(sh->paths);
		sh->env = export_env(sh);
		sh->paths = get_paths(sh->env);
	}
	last = iterate_cmds(cmds, sh);
	signaled = wait_all(last, &last_status);
	handle_exit_status(sh, last, last_status, signaled);
	set_signals_main(sh);
	return (sh->exit_code);
}
