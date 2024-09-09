/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 23:06:49 by jsommet           #+#    #+#             */
/*   Updated: 2024/09/09 15:43:29 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "quoicoushell.h"

static void	save_history(t_shell *sh)
{
	int		fd;
	t_list	*tmp;

	fd = open(sh->hist_file, O_WRONLY | O_CREAT | O_APPEND, 0600);
	if (fd >= -1)
	{
		tmp = sh->hist;
		while (tmp)
		{
			ft_putendl_fd((char *)tmp->content, fd);
			tmp = tmp->next;
		}
		close(fd);
	}
	ft_lstclear(&sh->hist, &free);
	free(sh->hist_file);
}

void	get_history(t_shell *sh)
{
	int		fd;
	char	*line;
	char	*home;

	home = get_variable_value(sh, "HOME");
	if (!home)
		return ;
	sh->hist_file = ft_strreplace(HISTORY_FILE, "$HOME", home,
		0x8000000000000000);
	set_variable(sh, ft_strdup("HISTFILE"), home, LST_ENV);
	fd = open(sh->hist_file, O_RDONLY);
	if (fd != -1)
	{
		line = get_next_line(fd);
		while (line)
		{
			line[ft_strlen(line) - 1] = 0;
			add_history(line);
			free(line);
			line = get_next_line(fd);
		}
		close(fd);
	}
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
	/* if (sh->fd_hist > -1) */
	/* 	close(sh->fd_hist); */
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
