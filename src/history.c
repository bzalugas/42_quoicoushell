/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:59:11 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/08 13:53:08 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

void	save_history(t_shell *sh)
{
	int		fd;
	t_list	*tmp;

	if (!sh->hist_file || sh->n_hist < 1)
		return ;
	fd = open(sh->hist_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd > -1)
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

int	get_history(t_shell *sh)
{
	int		fd;
	char	*line;

	sh->hist_file = expand_fhd(sh, HISTORY_FILE);
	if (!sh->hist_file)
		return (0);
	set_variable(sh, ft_strdup("HISTFILE"), ft_strdup(sh->hist_file), LST_ENV);
	fd = open(sh->hist_file, O_RDONLY | O_CREAT, 0600);
	if (fd == -1)
		return (0);
	line = get_next_line(fd);
	while (line && sh->n_hist < HISTORY_MAX_LINES)
	{
		line[ft_strlen(line) - 1] = 0;
		put_history(sh, line);
		line = get_next_line(fd);
	}
	close(fd);
	return (1);
}

int	put_history(t_shell *sh, char *line)
{
	add_history(line);
	if (!ft_lstadd_back(&sh->hist, ft_lstnew(line)))
		return (0);
	if (sh->n_hist == HISTORY_MAX_LINES)
		ft_lstremove(&sh->hist, sh->hist, &free);
	else
		sh->n_hist++;
	return (1);
}
