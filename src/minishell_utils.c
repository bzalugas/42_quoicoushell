/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 23:06:49 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/28 22:33:10 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

//TODO: might need to split these into two functions
void	exit_shell(t_shell *sh, int exit_code, bool display)
{
	if (display)
	{
		ft_putstr_fd("exit\n", 1);
	}
	/* else if (exit_code == EXIT_FAILURE) */
	/* { */
	/* 	perror("idk yet"); */
	/* } */
	ft_lstclear(&sh->local_vars, (&free_variable));
	ft_lstclear(&sh->env_vars, (&free_variable));
	free(sh->cwd);
	free(sh->prompt);
	free_split(sh->env);
	free_split(sh->paths);
	exit(exit_code);
}

char	*current_dir_name(t_shell *sh, int depth)
{
	int		i;
	char	*dir;
	char	**pwd_split;

	if (depth == 0)
		return (NULL);
	pwd_split = ft_split(sh->cwd, '/');
	if (!pwd_split)
		exit_shell(sh, EXIT_FAILURE, false);
	if (!pwd_split[0])
		return (NULL);
	i = 0;
	while (pwd_split[i])
		i++;
	i -= depth;
	dir = NULL;
	while (pwd_split[i])
	{
		dir = ft_strjoin_free(dir, ft_strjoin(pwd_split[i++], "/"), 1, 0);
		if (!dir)
			return (free_split(pwd_split), free(dir), NULL);
	}
	return (free_split(pwd_split), dir);
}

char	*build_prompt(t_shell *sh)
{
	char	*prompt;

	prompt = current_dir_name(sh, 2);
	prompt = ft_strjoin_free(prompt, "$ ", 1, 0);
	return (prompt);
}
