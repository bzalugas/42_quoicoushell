/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 23:08:46 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/30 16:45:18 by bazaluga         ###   ########.fr       */
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
					return ((*i > 1));
				j++;
			}
		}
		else if (*i > 1)
			return (true);
		else
			return (false);
		(*i)++;
	}
	return ((*i > 1));
}

int	ft_echo(t_shell *sh, t_cmd *cmd)
{
	int		i;
	bool	nl;

	sh->exit_code = 0;
	nl = !dash_n_option(cmd, &i);
	while (cmd->argv[i] && !sh->exit_code)
	{
		sh->exit_code = (sh->exit_code || !ft_putstr_fd(cmd->argv[i],
					STDOUT_FILENO));
		if (cmd->argv[i + 1] && !sh->exit_code)
			sh->exit_code = (sh->exit_code
					|| !ft_putchar_fd(' ', STDOUT_FILENO));
		i++;
	}
	if (nl && !sh->exit_code)
		sh->exit_code = (sh->exit_code || !ft_putchar_fd('\n', STDOUT_FILENO));
	if (sh->exit_code)
		perror("quoicoushell: echo: write error");
	return (sh->exit_code);
}
