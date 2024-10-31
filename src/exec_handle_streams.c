/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle_streams.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 15:31:57 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/31 19:07:03 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	ft_close_all(t_lstcmds *cmds)
{
	int		i;
	t_list	*node;

	i = -1;
	while (++i < 2)
	{
		if (cmds->fd[i][0] != -1)
			ft_close(cmds, cmds->fd[i][0]);
		if (cmds->fd[i][1] != -1)
			ft_close(cmds, cmds->fd[i][1]);
	}
	node = cmds->cmds;
	while (node && node->content)
	{
		if (((t_cmd *)(node->content))->fd_hd[0] != -1)
			ft_close(cmds, ((t_cmd *)(node->content))->fd_hd[0]);
		if (((t_cmd *)(node->content))->fd_hd[1] != -1)
			ft_close(cmds, ((t_cmd *)(node->content))->fd_hd[1]);
		node = node->next;
	}
	return (0);
}

int	ft_close(t_lstcmds *cmds, int fd)
{
	t_list	*node;

	if (fd == -1 || !cmds)
		return (-1);
	if (cmds->fd[0][0] == fd)
		cmds->fd[0][0] = -1;
	if (cmds->fd[0][1] == fd)
		cmds->fd[0][1] = -1;
	if (cmds->fd[1][0] == fd)
		cmds->fd[1][0] = -1;
	if (cmds->fd[1][1] == fd)
		cmds->fd[1][1] = -1;
	node = cmds->cmds;
	while (node && node->content)
	{
		if (((t_cmd *)(node->content))->fd_hd[0] == fd)
			((t_cmd *)(node->content))->fd_hd[0] = -1;
		if (((t_cmd *)(node->content))->fd_hd[1] == fd)
			((t_cmd *)(node->content))->fd_hd[1] = -1;
		node = node->next;
	}
	return (close(fd));
}

static int	get_infile(t_shell *sh, t_cmd *cmd, char *filename)
{
	int	fdn;
	int	tmp_fd;

	fdn = cmd->n_cmd % 2;
	if (!cmd->heredoc)
		ft_close(sh->cmds, sh->cmds->fd[fdn][0]);
	tmp_fd = open(filename, O_RDONLY);
	if (tmp_fd == -1)
	{
		print_perror(filename, NULL);
		return (-1);
	}
	if (!cmd->heredoc)
		sh->cmds->fd[fdn][0] = tmp_fd;
	return (0);
}

static int	get_outfile(t_shell *sh, t_cmd *cmd, int redir_i)
{
	int		fdn;
	t_redir	redir;

	redir = cmd->redirs[redir_i];
	fdn = (cmd->n_cmd + 1) % 2;
	ft_close(sh->cmds, sh->cmds->fd[fdn][1]);
	if (redir.type == RTOUT_A)
		sh->cmds->fd[fdn][1] = open(redir.file, O_WRONLY | O_CREAT | O_APPEND,
				0644);
	else
		sh->cmds->fd[fdn][1] = open(redir.file, O_WRONLY | O_CREAT | O_TRUNC,
				0644);
	if (sh->cmds->fd[fdn][1] == -1)
	{
		print_perror(redir.file, NULL);
		return (-1);
	}
	return (0);
}

int	get_in_out_files(t_shell *sh, t_cmd *cmd)
{
	int	i;
	int	problem;

	if (!cmd->redirs)
		return (-1);
	i = 0;
	while (cmd->redirs[i].file)
	{
		if (cmd->redirs[i].type == RTIN)
			problem = get_infile(sh, cmd, cmd->redirs[i].file);
		else
			problem = get_outfile(sh, cmd, i);
		if (problem == -1)
			return (ft_close_all(sh->cmds), -1);
		i++;
	}
	return (0);
}
