/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 12:38:03 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/10 12:46:07 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

static int	run_non_builtin(t_lstcmds *cmds, t_cmd *cmd, t_shell *sh)
{
	size_t	i;
	char	*abs_cmd;

	if (!cmd->argv[0])
		exit_shell(sh, EXIT_SUCCESS, false);
	if (ft_strchr(cmd->argv[0], '/'))
		if (execve(cmd->argv[0], cmd->argv, sh->env))
			stop_perror(cmd->argv[0], 0, cmds, sh);
	i = 0;
	while (sh->paths && sh->paths[i])
	{
		abs_cmd = ft_strjoin(sh->paths[i], cmd->argv[0]);
		execve(abs_cmd, cmd->argv, sh->env);
		free(abs_cmd);
		i++;
	}
	return (stop_error(cmd->argv[0], 127, cmds, sh));
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
		get_in_out_files(sh, cmd, true);
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
		g_sig = 0;
		cmd = node_cmd->content;
		if (g_sig == SIGINT)
			return (last);
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

	if (cmds->n_cmds == 1 && !((t_cmd *)(cmds->cmds->content))->argv[0])
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
	if (last != -1)
		waitpid(last, &sh->exit_code, 0);
	while (errno != ECHILD)
		wait(NULL);
	if (last != -1)
		sh->exit_code = WEXITSTATUS(sh->exit_code);
	set_signals_main(sh);
	return (sh->exit_code);
}
