/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 19:41:42 by bazaluga          #+#    #+#             */
/*   Updated: 2024/09/17 13:00:13 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

bool valid_var_name(char *name)
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

int var_error(char *arg)
{
	char	*buf;

	buf = ft_strjoin_free(ft_strjoin("quoicoushell: export: `", arg),
		"': not a valid identifier", 1, 0);
	ft_putendl_fd(buf, STDERR_FILENO);
	free(buf);
	return (1);
}

int	cut_local_exports(t_cmd *cmd, int start_cmd)
{
	int	i;

	i = 0;
	while (cmd->argv[i])
	{
		cmd->argv[i] = cmd->argv[start_cmd];
		//move all ptrs from argv[start_cmd] to 0->end (don't forget to free)
		i++;
	}
}
