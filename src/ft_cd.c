/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 20:03:08 by bazaluga          #+#    #+#             */
/*   Updated: 2024/09/27 12:38:22 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

static int	cd_get_path(t_cmd *cmd, t_shell *sh, char **path)
{
	if (!cmd->argv[1])
	{
		*path = get_variable_value(sh, "HOME");
		if (!*path)
			return (1);
	}
	else if (!ft_strcmp(cmd->argv[1], "-"))
	{
		*path = get_variable_value(sh, "OLDPWD");
		if (!*path)
			return (2);
	}
	else
		*path = cmd->argv[1];
	return (0);
}

int	ft_cd(t_cmd *cmd, t_shell *sh)
{
	char	*path;
	int		res;

	if (cmd->argc > 2)
		return (print_error(cmd->argv[0], "too many arguments"));
	res = cd_get_path(cmd, sh, &path);
	if (res == 1)
		print_error(cmd->argv[0], "HOME not set");
	else if (res == 2)
		print_error(cmd->argv[0], "OLDPWD not set");
	else if (chdir(path) == -1)
		print_perror(cmd->argv[0], path);
	set_variable(sh, ft_strdup("OLDPWD"), ft_strdup(sh->cwd), LST_ENV);
	free(sh->cwd);
	sh->cwd = getcwd(NULL, 0);
	if (!sh->cwd)
		print_perror("chdir: error retrieving current directory", "getcwd");
	set_variable(sh, ft_strdup("PWD"), ft_strdup(sh->cwd), LST_ENV);
	free(sh->prompt);
	sh->prompt = build_prompt(sh);
	sh->env_update = true;
	sh->exit_code = 0;
	return (0);
}
