/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 20:03:08 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/28 21:29:50 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

static char	*cd_get_path(t_cmd *cmd, t_shell *sh)
{
	char	*path;

	if (!cmd->argv[1])
	{
		path = get_variable_value(sh, "HOME");
		return (path);
	}
	return (cmd->argv[1]);
}

int	ft_cd(t_cmd *cmd, t_shell *sh)
{
	char	*path;

	path = cd_get_path(cmd, sh);
	if (!path)
		return (1);
	if (chdir(path) == -1)
	{
		print_perror(cmd->argv[0], cmd->argv[1]);
		return (-1);
	}
	free(sh->cwd);
	sh->cwd = getcwd(NULL, 0);
	sh->prompt = build_prompt(sh);
	sh->exit_code = 0;
	return (0);
}
