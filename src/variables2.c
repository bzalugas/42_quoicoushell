/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 05:24:13 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/17 17:29:35 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

t_list	*set_variable(t_shell *sh, char *name, char *new_value)
{
	t_list	*link;

	link = get_variable(sh, name);
	if (!link)
		return (NULL);
	free(((t_var *)link->content)->value);
	((t_var *)link->content)->value = new_value;
	return (link);
}

t_list	*get_variable(t_shell *sh, char	*name)
{
	t_list	*link;

	link = sh->local_vars;
	while (link)
	{
		if (link->content && !strcmp(((t_var *)link->content)->name, name))
			return (link);
		link = link->next;
	}
	link = sh->env_vars;
	while (link)
	{
		if (link->content && !strcmp(((t_var *)link->content)->name, name))
			return (link);
		link = link->next;
	}
	return (NULL);
}

t_var	*new_variable(char *name, char *value)
{
	t_var	*var;

	var = (t_var *) malloc(sizeof(t_var));
	if (!var)
		return (NULL);
	var->name = name;
	var->value = value;
	return (var);
}

void	free_variable(void *var)
{
	free(((t_var *) var)->name);
	free(((t_var *) var)->value);
	free(var);
}

char	*get_variable_value(t_shell *sh, char *name)
{
	t_list	*link;
	char	*value;

	link = get_variable(sh, name);
	value = ((t_var *)link->content)->value;
	return (value);
}
