/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 23:08:46 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/15 22:21:24 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

static bool	dash_n_option(t_cmd *cmd, int *i)
{
	int		j;

	*i = 1;
	while (cmd->argv[*i])
	{
		j = 0;
		if (!ft_strncmp(cmd->argv[*i], "-n", 2))
		{
			j = 2;
			while (cmd->argv[*i][j])
			{
				if (cmd->argv[*i][j] != 'n')
					return (false);
				j++;
			}
		}
		else if (*i > 1)
			return (true);
		else
			return (false);
		(*i)++;
	}
	return (false);
}

int	ft_echo(t_shell *sh, t_cmd *cmd)
{
	int		i;
	bool	nl;

	sh->exit_code = 0;
	nl = !dash_n_option(cmd, &i);
	while (cmd->argv[i] && !sh->exit_code)
	{
		sh->exit_code = (sh->exit_code || (write(STDOUT_FILENO, cmd->argv[i],
						ft_strlen(cmd->argv[i])) == -1));
		if (cmd->argv[i + 1] && !sh->exit_code)
			sh->exit_code = (sh->exit_code
					|| (write(STDOUT_FILENO, " ", 1) == -1));
		i++;
	}
	if (nl && !sh->exit_code)
		sh->exit_code = (sh->exit_code || (write(STDOUT_FILENO, "\n", 1) == -1));
	if (sh->exit_code)
		perror("quoicoushell: echo: write error");
	return (sh->exit_code);
}
