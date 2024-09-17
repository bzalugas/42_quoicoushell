/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 23:06:49 by jsommet           #+#    #+#             */
/*   Updated: 2024/09/17 18:52:50 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "quoicoushell.h"

void	exit_shell(t_shell *sh, int exit_code, bool display)
{
	if (display)
		ft_putstr_fd("exit\n", sh->tty_fd);
	ft_lstclear(&sh->local_vars, (&free_variable));
	ft_lstclear(&sh->env_vars, (&free_variable));
	free(sh->cwd);
	free(sh->prompt);
	free_split(sh->env);
	free_split(sh->paths);
	free_cmds(sh->cmds);
	if (sh->stdout_fd != -1)
		close(sh->stdout_fd);
	if (sh->tty_fd != -1)
		close(sh->tty_fd);
	save_history(sh);
	exit(exit_code);
}

char	*current_dir_name(t_shell *sh, int depth)
{
	int		i;
	char	*dir;
	char	**pwd_split;

	if (depth == 0 || !sh->cwd)
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
		dir = ft_strjoin_free(dir, ft_strjoin(pwd_split[i++], "/"), 1, 1);
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
