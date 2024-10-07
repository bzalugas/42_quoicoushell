/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 23:06:49 by jsommet           #+#    #+#             */
/*   Updated: 2024/10/07 13:25:30 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "quoicoushell.h"
#include <stdbool.h>
#include <stdlib.h>

int	exit_shell(t_shell *sh, int exit_code, bool display)
{
	if (display)
		ft_putstr_fd("exit\n", STDOUT_FILENO);
	ft_lstclear(&sh->local_vars, (&free_variable));
	ft_lstclear(&sh->env_vars, (&free_variable));
	free(sh->cwd);
	free(sh->prompt);
	free_split(sh->env);
	free_split(sh->paths);
	free_cmds(sh->cmds);
	save_history(sh);
	exit(exit_code);
	return (exit_code);
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

char	*readline_fd(t_shell *sh)
{
	char	*line;
	int		is_tty;

	(void)sh;
	is_tty = 1;
	if (!isatty(STDOUT_FILENO))
		is_tty = 0;
	if (!is_tty)
		if (dup2(get_tty(), STDOUT_FILENO) == -1)
			return (NULL);
			/* exit_shell(sh, EXIT_FAILURE, true); */
	line = readline(sh->prompt);
	/* dprintf(2, "line = <%s>\n", line); */
	if (!is_tty)
		if (dup2(get_stdout(), STDOUT_FILENO) == -1)
			return (NULL);
	return (line);
}

int	stop_main_error(t_shell *sh, char *msg, int error)
{
	ft_dprintf(STDERR_FILENO, "%s\n", msg);
	return (exit_shell(sh, error, true));
}
