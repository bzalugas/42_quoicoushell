/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 05:24:13 by jsommet           #+#    #+#             */
/*   Updated: 2024/09/15 19:03:23 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

t_list	*set_variable_value(t_shell *sh, char *name, char *new_value)
{
	t_list	*link;

	link = get_variable(sh, name, LST_BOTH);
	if (!link)
		return (NULL);
	free(((t_var *)link->content)->value);
	((t_var *)link->content)->value = new_value;
	return (link);
}

t_list	*get_variable(t_shell *sh, char	*name, int where)
{
	t_list	*link;
	int		i;

	i = 0;
	while (i < 2)
	{
		if (where != LST_BOTH && i > 0)
			break ;
		else if (where == LST_LOCAL || (where == LST_BOTH && i == 0))
			link = sh->local_vars;
		else if (where == LST_ENV || (where == LST_BOTH && i == 1))
			link = sh->env_vars;
		while (link)
		{
			if (link->content && !strcmp(((t_var *)link->content)->name, name))
				return (link);
			link = link->next;
		}
		i++;
	}
	return (NULL);
}

char	*get_variable_value(t_shell *sh, char *name)
{
	t_list	*link;
	char	*value;

	link = get_variable(sh, name, LST_BOTH);
	if (!link)
		return (NULL);
	value = ((t_var *)link->content)->value;
	return (value);
}
