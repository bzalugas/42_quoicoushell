/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:59:11 by bazaluga          #+#    #+#             */
/*   Updated: 2024/09/30 18:04:34 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

void	save_history(t_shell *sh)
{
	int		fd;
	t_list	*tmp;

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

void	get_history(t_shell *sh)
{
	int		fd;
	char	*line;

	sh->hist_file = expand_fhd(sh, HISTORY_FILE);
	set_variable(sh, ft_strdup("HISTFILE"), ft_strdup(sh->hist_file), LST_ENV);
	fd = open(sh->hist_file, O_RDONLY);
	if (fd == -1)
		return ;
	line = get_next_line(fd);
	while (line && sh->n_hist < HISTORY_MAX_LINES)
	{
		line[ft_strlen(line) - 1] = 0;
		put_history(sh, line);
		line = get_next_line(fd);
	}
	close(fd);
}

void	put_history(t_shell *sh, char *line)
{
	add_history(line);
	if (!ft_lstadd_back(&sh->hist, ft_lstnew(line)))
		return ;
	if (sh->n_hist == HISTORY_MAX_LINES)
		ft_lstremove(&sh->hist, sh->hist, &free);
	else
		sh->n_hist++;
}
