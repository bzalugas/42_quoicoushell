/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 06:24:12 by jsommet           #+#    #+#             */
/*   Updated: 2024/10/18 23:00:26 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

static void	redirect_streams(t_lstcmds *cmds, t_cmd *cmd, int tmp_fds[2])
{
	int		fd_in;
	int		fd_out;

	tmp_fds[0] = -1;
	tmp_fds[1] = -1;
	fd_in = cmds->fd[cmd->n_cmd % 2][0];
	fd_out = cmds->fd[(cmd->n_cmd + 1) % 2][1];
	if (fd_in > -1)
	{
		tmp_fds[0] = dup(STDIN_FILENO);
		dup2(fd_in, STDIN_FILENO);
		ft_close(cmds, fd_in);
	}
	if (fd_out > -1)
	{
		tmp_fds[1] = dup(STDOUT_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		ft_close(cmds, fd_out);
	}
}

static void	get_back_streams(int fds[2])
{
	if (fds[0] > -1)
	{
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
	}
	if (fds[1] > -1)
	{
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
	}
}

static bool	is_builtin(t_cmd *cmd)
{
	if (!ft_strcmp(cmd->argv[0], "echo") || !ft_strcmp(cmd->argv[0], "cd")
		|| !ft_strcmp(cmd->argv[0], "pwd") || !ft_strcmp(cmd->argv[0], "export")
		|| !ft_strcmp(cmd->argv[0], "unset") || !ft_strcmp(cmd->argv[0], "env")
		|| !ft_strcmp(cmd->argv[0], "exit") || ft_strchr(cmd->argv[0], '='))
		return (true);
	return (false);
}

static int	run_right_builtin(t_cmd *cmd, t_shell *sh)
{
	int	res;

	if (!ft_strcmp(cmd->argv[0], "echo"))
		ft_echo(sh, cmd);
	else if (!ft_strcmp(cmd->argv[0], "cd"))
		ft_cd(cmd, sh);
	else if (!ft_strcmp(cmd->argv[0], "pwd"))
		ft_pwd(sh);
	else if (!ft_strcmp(cmd->argv[0], "export"))
		ft_export(cmd, sh);
	else if (!ft_strcmp(cmd->argv[0], "unset"))
		ft_unset(cmd, sh);
	else if (!ft_strcmp(cmd->argv[0], "env"))
		ft_env(sh);
	else if (!ft_strcmp(cmd->argv[0], "exit"))
		ft_exit(cmd, sh);
	else
	{
		res = ft_local_export(cmd, sh);
		if (res == 0)
			return (1);
		else if (res > 0)
			return (cut_local_exports(cmd, res));
	}
	return (1);
}

int	run_builtin(t_lstcmds *cmds, t_cmd *cmd, t_shell *sh, bool forked)
{
	int	tmp_fds[2];
	int	res;

	if (!cmd->argv[0] || (cmds->n_cmds > 1 && !forked) || !is_builtin(cmd))
		return (0);
	if (get_in_out_files(sh, cmd, forked) == -1)
		return (1);
	redirect_streams(cmds, cmd, tmp_fds);
	res = run_right_builtin(cmd, sh);
	if (res == 0 || forked)
		exit_shell(sh, sh->exit_code, false);
	get_back_streams(tmp_fds);
	return (res);
}
