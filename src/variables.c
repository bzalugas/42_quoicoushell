/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 01:10:28 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/23 22:40:53 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

/*
* @brief Imports env variables from envp as a linked list.
* Adds those variables to sh->env_vars.
* @return Pointer to the first link of sh->env_vars.
*/
t_list	*import_env(t_shell *sh, char **envp)
{
	int		i;
	char	**entry_split;

	i = 0;
	while (envp[i])
	{
		entry_split = split_env_entry(envp[i]);
		dprintf(2, "0 %s\n", entry_split[0]);
		dprintf(2, "1 %s\n", entry_split[1]);
		if (!entry_split)
			return (NULL);
		if (!add_variable(sh, entry_split[0], entry_split[1], 1))
			return (free_split(entry_split), NULL);
		free(entry_split);
		i++;
	}
	return (sh->env_vars);
}

char	**export_env(t_shell *sh)
{
	int		i;
	t_list	*env;
	char	**envx;

	env = sh->env_vars;
	if (!env)
		return (NULL);
	envx = (char **) calloc(ft_lstsize(env), sizeof(char *));
	if (!envx)
		return (NULL);
	i = 0;
	while (env)
	{
		envx[i] = ft_strjoin(((t_var *)env->content)->name, "=");
		if (!envx[i])
			return (free_split(envx), NULL);
		envx[i] = ft_strjoin_free(envx[i],
				((t_var *)env->content)->value, 1, 0);
		if (!envx[i])
			return (free_split(envx), NULL);
		env = env->next;
		i++;
	}
	return (envx);
}

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
		return (NULL);
	ft_lstunlink(&sh->local_vars, link);
	ft_lstadd_back(&sh->env_vars, link);
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
	if (new)
		return (new);
	var = new_variable(name, value);
	if (!var)
		return (NULL);
	new = ft_lstnew(var);
	if (!new)
		return (free_variable(var), NULL);
	ft_lstadd_back(&sh->local_vars, new);
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
