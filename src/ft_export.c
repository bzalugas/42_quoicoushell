/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 14:13:44 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/23 16:15:33 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

static bool valid_var_name(char *name)
{
	size_t	i;

	if (!name)
		return (false);
	i = 0;
	while (name[i])
	{
		if (!ft_isalnum(name[i]))
			return (false);
		i++;
	}
	return (true);
}

static int var_error(char *arg)
{
	char	*buf;

	buf = ft_strjoin_free(ft_strjoin("quoicoushell: export: `", arg),
		"': not a valid identifier", 1, 0);
	ft_putendl_fd(buf, STDERR_FILENO);
	free(buf);
	return (1);
}

int	ft_export(t_lstcmds *cmds, t_cmd *cmd, t_shell *sh)
{
	char	**args;

	args = split_env_entry(cmd->argv[1]);
	if (!args)
	{
		if (cmd->n_cmd > 0)
			return (stop_error("export", 1, cmds));
		exit_shell(sh, 1);
	}
	if (!valid_var_name(args[0]))
		return (free_split(args), var_error(cmd->argv[1]));
	if (args[1] == NULL)
		export_variable(sh, args[0]);
	else
		add_variable(sh, args[0], args[1], 1);
	free(args);
	return (0);
}
