/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 15:41:53 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/27 18:30:31 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

char	**split_env_entry(char *entry)
{
	char	**split;
	int		i;

	split = (char **) calloc(3, sizeof(char *));
	i = 0;
	while (entry[i] && entry[i] != '=')
		i++;
	split[0] = ft_substr(entry, 0, i);
	if (!entry[i])
		split[1] = NULL;
	else
		split[1] = ft_strdup(&entry[i + 1]);
	return (split);
}

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
		/* dprintf(2, "0 %s\n", entry_split[0]); */
		/* dprintf(2, "1 %s\n", entry_split[1]); */
		if (!entry_split)
			return (NULL);
		if (!add_variable(sh, entry_split[0], entry_split[1], 1))
			return (free_split(entry_split), NULL);
		free(entry_split);
		i++;
	}
	return (sh->env_vars);
}

static char *str_var_env(t_var *var, bool add_quotes)
{
	char	*str;
	size_t	len_name;
	size_t	len_value;

	len_name = ft_strlen(var->name);
	len_value = ft_strlen(var->value);
	str = ft_calloc(len_name + len_value + (add_quotes * 2) + 2, sizeof(char));
	if (!str)
		return (NULL);
	ft_strlcat(str, var->name, len_name + 1);
	if (var->value)
	{
		ft_strlcat(str, "=", len_name + 2);
		if (add_quotes)
			ft_strlcat(str, "\"", len_name + 3);
		ft_strlcat(str, var->value, len_name + len_value + 1 + add_quotes + 1);
		if (add_quotes)
			ft_strlcat(str, "\"", len_name + len_value + 4);
	}
	return (str);
}

char	**export_env(t_shell *sh)
{
	int		i;
	t_list	*env;
	char	**envx;

	env = sh->env_vars;
	if (!env)
		return (NULL);
	envx = (char **)ft_calloc(ft_lstsize(env) + 1, sizeof(char *));
	if (!envx)
		return (NULL);
	i = 0;
	while (env)
	{
		if (((t_var*)env->content)->value)
		{
			envx[i] = str_var_env((t_var *)env->content, false);
			if (!envx[i])
				return (free_split(envx), NULL);
		}
		env = env->next;
		i++;
	}
	return (envx);
}

char	**export_all_env(t_shell *sh)
{
	t_list	*env;
	char	**envx;
	int		i;

	env = sh->env_vars;
	if (!env)
		return (NULL);
	envx = (char**)ft_calloc(ft_lstsize(env) + 1, sizeof(char *));
	if (!envx)
		return (NULL);
	i = 0;
	while (env)
	{
		envx[i] = ft_strjoin_free("export ",
			str_var_env((t_var *)env->content, true), 0, 1);
		if (!envx[i])
			return (free_split(envx), NULL);
		env = env->next;
		i++;
	}
	return (envx);
}
