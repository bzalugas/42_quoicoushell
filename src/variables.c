/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 01:10:28 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/27 18:49:23 by bazaluga         ###   ########.fr       */
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

	link = get_variable(sh, name);
	if (!link)
		link = add_variable(sh, name, NULL, 1);
	else
	{
		ft_lstunlink(&sh->local_vars, link);
		ft_lstadd_back(&sh->env_vars, link);
	}
	return (link);
}

/*
 * @brief Adds variable to either sh->local_vars or sh->env-vars.
 * If there already is a variable with this name,
 * it edits the value of the existing variable.
 * @return A pointer to the new variable. If the variable already exists,
 *  it returns a pointer to
 * the existing link. Returns NULL if somthing goes wrong.
 * @param name The name of the new variable.
 * @param value The value of the new variable.
 * @param env Boolean, decides wether to add variable to local or env_vars.
 */
t_list	*add_variable(t_shell *sh, char *name, char *value, int env)
{
	t_list	*new;
	t_var	*var;

	new = set_variable(sh, name, value);
	if (!new)
	{
		var = new_variable(name, value);
		if (!var)
			return (NULL);
		new = ft_lstnew(var);
		if (!new)
			return (free_variable(var), NULL);
		ft_lstadd_back(&sh->local_vars, new);
	}
	if (env)
		export_variable(sh, name);
	return (new);
}

int	del_variable(t_shell *sh, char *name)
{
	t_list	*link;

	link = get_variable(sh, name);
	if (!link)
		return (-1);
	ft_lstremove(&sh->local_vars, link, &free_variable);
	ft_lstremove(&sh->env_vars, link, &free_variable);
	return (0);
}
