/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 14:13:44 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/28 19:53:14 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

static int	ft_alone_export(t_shell *sh)
{
	int		i;
	char	**env;

	env = export_all_env(sh);
	if (!env)
		return (sh->exit_code = 1, 1);
	i = 0;
	while (env[i])
	{
		ft_putendl_fd(env[i], STDOUT_FILENO);
		i++;
	}
	sh->exit_code = 0;
	return (0);
}

static int	iterate_exports(char **argv, t_shell *sh)
{
	int		i;
	char	**args;
	t_list	*link;

	i = 1;
	while (argv[i])
	{
		args = split_env_entry(argv[i]);
		if (!args)
			exit_shell(sh, 1);
		if (!valid_var_name(args[0]))
			return (free_split(args), var_error(argv[i]));
		if (args[1])
			remove_quotes(args[1]);
		link = export_variable(sh, args[0]);
		if (link && args[1])
			((t_var *)link->content)->value = args[1];
		else if (!link)
			set_variable(sh, args[0], args[1], LST_ENV);
		free(args);
		i++;
	}
	return (0);
}

int	ft_export(t_cmd *cmd, t_shell *sh)
{
	if (!cmd->argv[1])
		return (ft_alone_export(sh));
	sh->exit_code = iterate_exports(cmd->argv, sh);
	sh->env_update = true;
	return (0);
}

//Need to handle a=2 b=5 c=23
int	ft_local_export(t_lstcmds *cmds, t_cmd *cmd, t_shell *sh)
{
	char	**args;

	args = split_env_entry(cmd->argv[0]);
	if (!args)
	{
		if (cmds->n_cmds > 1)
			return (stop_error("local_export", 1, cmds));
		exit_shell(sh, 1);
	}
	if (!valid_var_name(args[0]))
		return (free_split(args), var_error(cmd->argv[1]));
	if (!args[1])
		free_split(args);
	else
	{
		set_variable(sh, args[0], args[1], LST_LOCAL);
		free(args);
	}
	t_list	*first = sh->local_vars;
	while (first)
	{
		ft_printf("%s=%s\n", ((t_var *)first->content)->name,
			((t_var *)first->content)->value);
		first = first->next;
	}
	sh->exit_code = 0;
	return (0);
}
