/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 12:38:03 by bazaluga          #+#    #+#             */
/*   Updated: 2024/09/17 19:33:11 by bazaluga         ###   ########.fr       */
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

	fd_in = cmds->fd[cmd->n_cmd % 2][0];
	fd_out = cmds->fd[(cmd->n_cmd + 1) % 2][1];
	if (fd_in > -1)
		dup2(fd_in, STDIN_FILENO);
	if (fd_out > -1)
		dup2(fd_out, STDOUT_FILENO);
	ft_close(cmds, fd_in);
	ft_close(cmds, fd_out);
	status = run_builtin(cmds, cmd, sh, true);
	if (status)
		exit(sh->exit_code);
	return (run_non_builtin(cmds, cmd, sh));
}

static int	prepare_run_cmd(t_lstcmds *cmds, t_cmd *cmd, t_shell *sh)
{
	pid_t	pid;
	int		pipe_in;
	int		pipe_out;

	if (cmds->n_cmds == 1 && run_builtin(cmds, cmd, sh, false) == 1)
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
		get_in_out_files(sh, cmd, true);
		return (run_cmd(cmds, cmd, sh));
	}
	sh->sa.sa_handler = &signal_handler_other;
	sigaction(SIGINT, &sh->sa, &sh->sa_tmp);
	ft_close(cmds, cmds->fd[pipe_in][0]);
	ft_close(cmds, cmds->fd[pipe_in][1]);
	if (cmd->hd_filename)
		unlink(cmd->hd_filename);
	return (pid);
}

static int	iterate_cmds(t_lstcmds *cmds, t_shell *sh)
{
	t_list	*node_cmd;
	t_cmd	*cmd;
	pid_t	last;

	last = -1;
	node_cmd = cmds->cmds;
	while (node_cmd && node_cmd->content)
	{
		g_sigint = 0;
		cmd = node_cmd->content;
		sh->sa.sa_handler = &signal_handler_other;
		sigaction(SIGINT, &sh->sa, &sh->sa_tmp);
		get_heredocs(cmds, cmd, sh);
		sigaction(SIGINT, &sh->sa_tmp, NULL);
		if (g_sigint == 1)
			return (last);
		if (cmd->n_cmd < cmds->n_cmds - 1)
			if (pipe(cmds->fd[(cmd->n_cmd + 1) % 2]) == -1)
				exit(errno);
		last = prepare_run_cmd(cmds, cmd, sh);
		node_cmd = node_cmd->next;
	}
	return (last);
}

int	run_all_cmds(t_lstcmds *cmds, t_shell *sh)
{
	pid_t	last;

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
	sigaction(SIGINT, &sh->sa_tmp, NULL);
	return (sh->exit_code);
}
