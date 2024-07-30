/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 23:06:49 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/31 00:04:12 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

void	save_history(t_shell *sh, char *line)
{
	if (sh->fd_hist == -1)
		return ;
	ft_putendl_fd(line, sh->fd_hist);
}

void	get_history(t_shell *sh)
{
	char	*file;
	char	*line;
	char	*home;

	home = ft_strjoin(get_variable_value(sh, "HOME"), "/");
	file = ft_strreplace(HISTORY_FILE, "$HOME", home);
	free(home);
	ft_dprintf(2, "<%s>\n", file);
	sh->fd_hist = open(file, O_RDONLY);
	if (sh->fd_hist != -1)
	{
		line = get_next_line(sh->fd_hist);
		while (line)
		{
			add_history(line);
			free(line);
			line = get_next_line(sh->fd_hist);
		}
		close(sh->fd_hist);
	}
	sh->fd_hist = open(file, O_WRONLY | O_CREAT | O_APPEND, 0600);
	free(file);
}

void	exit_shell(t_shell *sh, int exit_code, bool display)
{
	if (display)
		ft_putstr_fd("exit\n", 1);
	ft_lstclear(&sh->local_vars, (&free_variable));
	ft_lstclear(&sh->env_vars, (&free_variable));
	free(sh->cwd);
	free(sh->prompt);
	free_split(sh->env);
	free_split(sh->paths);
	free_cmds(sh->cmds);
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
