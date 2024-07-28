/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 06:24:12 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/28 23:17:20 by bazaluga         ###   ########.fr       */
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

static int	run_right_builtin(t_lstcmds *cmds, t_cmd *cmd, t_shell *sh)
{
	if (!ft_strcmp(cmd->argv[0], "echo"))
		ft_echo(cmd);
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
	else if (ft_local_export(cmds, cmd, sh) == 0)
		return (1);
	else
		return (0);
	return (1);
}

//TODO: Correctly return/exit functions if in fork or not
int	run_builtin(t_lstcmds *cmds, t_cmd *cmd, t_shell *sh, bool forked)
{
	int	tmp_fds[2];

	if (!cmd->argv[0] || (cmds->n_cmds > 1 && !forked) || !is_builtin(cmd))
		return (0);
	get_in_out_files(cmds, cmd, forked);
	redirect_streams(cmds, cmd, tmp_fds);
	if (run_right_builtin(cmds, cmd, sh) == 0 || forked)
		exit(sh->exit_code);
	get_back_streams(tmp_fds);
	return (1);
}
