/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 19:41:42 by bazaluga          #+#    #+#             */
/*   Updated: 2024/09/17 17:55:07 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

bool valid_var_name(char *name)
{
	size_t	i;

	if (!name)
		return (false);
	i = 0;
	while (name[i] && name[i] != '=')
	{
		if (i == 0 && (name[i] >= '0' && name[i] <= '9'))
			return (false);
		else if (!ft_isalnum(name[i]))
			return (false);
		i++;
	}
	if (i == 0)
		return (false);
	return (true);
}

int var_error(char *arg, char local)
{
	if (local)
		ft_dprintf(STDERR_FILENO, "quoicoushell: %s: command not found\n", arg);
	else
		ft_dprintf(STDERR_FILENO,
			"quoicoushell: export: `%s': not a valid identifer\n", arg);
	return (1);
}

int	cut_local_exports(t_cmd *cmd, int start_cmd)
{
	int	i;

	i = 0;
	while (cmd->argv[start_cmd])
	{
		free(cmd->argv[i]);
		cmd->argv[i] = cmd->argv[start_cmd];
		cmd->argv[start_cmd] = NULL;
		i++;
		start_cmd++;
	}
	while (cmd->argv[i])
	{
		cmd->argv[i] = NULL;
		i++;
	}
	return (3);
}
