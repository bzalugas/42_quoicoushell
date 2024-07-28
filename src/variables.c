/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 01:10:28 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/28 19:31:01 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

/*
 * @brief Moves variable from sh->local_vars to sh->env_vars.
 * @return 0 on success,
 * -1 on error (ie: variable not found).
 */
t_list	*export_variable(t_shell *sh, char *name)
{
	t_list	*link;

	link = get_variable(sh, name, LST_ENV);
	if (!link)
	{
		link = get_variable(sh, name, LST_LOCAL);
		if (!link)
			return (NULL);
		ft_lstunlink(&sh->local_vars, link);
		ft_lstadd_back(&sh->env_vars, link);
	}
	return (link);
}

t_list	*set_variable(t_shell *sh, char *name, char *value, int where)
{
	t_list	*new;
	t_var	*var;

	new = set_variable_value(sh, name, value);
	if (!new)
	{
		var = new_variable(name, value);
		if (!var)
			return (NULL);
		new = ft_lstnew(var);
		if (!new)
			return (free_variable(var), NULL);
		if (where == LST_LOCAL)
			ft_lstadd_back(&sh->local_vars, new);
		else if (where == LST_ENV)
			ft_lstadd_back(&sh->env_vars, new);
	}
	return (new);
}

int	del_variable(t_shell *sh, char *name)
{
	t_list	*link;

	link = get_variable(sh, name, LST_BOTH);
	if (!link)
		return (-1);
	ft_lstremove(&sh->local_vars, link, &free_variable);
	ft_lstremove(&sh->env_vars, link, &free_variable);
	return (0);
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
