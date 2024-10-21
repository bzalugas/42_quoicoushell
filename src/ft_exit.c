/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 22:14:35 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/21 17:37:29 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

bool	is_numeric_argument(char *arg)
{
	size_t	i;

	i = 0;
	while (ft_isspace(arg[i]))
		i++;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (ft_isdigit(arg[i]))
		i++;
	while (arg[i])
	{
		if (!ft_isspace(arg[i]))
			return (false);
		i++;
	}
	return (true);
}

int	ft_exit(t_cmd *cmd, t_shell *sh)
{
	long long	exit_code;
	char		*end;

	if (!cmd->argv[1])
		exit_shell(sh, sh->exit_code, true);
	exit_code = ft_strtoll(cmd->argv[1], &end, 10);
	if (end == NULL || errno == ERANGE || !is_numeric_argument(cmd->argv[1]))
	{
		ft_dprintf(STDERR_FILENO,
			"exit\nminishell: exit: %s: numeric argument required\n",
			cmd->argv[1]);
		exit_shell(sh, 2, false);
	}
	if (cmd->argc > 2)
	{
		ft_dprintf(STDERR_FILENO,
			"exit\nminishell: exit: too many arguments\n");
		return (1);
	}
	exit_shell(sh, exit_code % 256, true);
	return (0);
}
