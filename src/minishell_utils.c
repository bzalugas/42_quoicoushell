/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 23:06:49 by jsommet           #+#    #+#             */
/*   Updated: 2024/10/29 18:10:42 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

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

char	*current_dir_name(t_shell *sh, size_t depth)
{
	int		i;
	char	*dir;
	char	**pwd_split;

	if (depth == 0 || !sh->cwd)
		return (NULL);
	pwd_split = ft_split(sh->cwd, '/');
	if (!pwd_split)
		exit_shell(sh, EXIT_FAILURE, false);
	dir = NULL;
	if (ft_splitlen(pwd_split) <= depth)
		dir = ft_strdup("/");
	i = 0;
	while (pwd_split[i])
		i++;
	i -= depth;
	i = (i > 0) * i;
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
	prompt = ft_strjoin_free(PROMPT_COLOUR6, prompt, 0, 1);
	prompt = ft_strjoin_free(prompt, PROMPT_END, 1, 0);
	prompt = ft_strjoin_free(prompt, "$ ", 1, 0);
	return (prompt);
}

char	*readline_fd(t_shell *sh)
{
	char	*line;
	int		is_tty;
	int		tty_fd;
	int		stdout_fd;

	is_tty = isatty(STDOUT_FILENO);
	if (!is_tty)
	{
		stdout_fd = dup(STDOUT_FILENO);
		tty_fd = open(ttyname(STDIN_FILENO), O_WRONLY);
		if (dup2(tty_fd, STDOUT_FILENO) == -1)
			return (NULL);
	}
	line = readline(sh->prompt);
	if (!is_tty)
	{
		if (dup2(stdout_fd, STDOUT_FILENO) == -1)
			return (NULL);
		close(stdout_fd);
		close(tty_fd);
	}
	return (line);
}

int	stop_main_error(t_shell *sh, char *msg, int error)
{
	ft_dprintf(STDERR_FILENO, "%s\n", msg);
	return (exit_shell(sh, error, true));
}
