/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_fhd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 19:13:14 by jsommet           #+#    #+#             */
/*   Updated: 2024/09/30 20:10:06 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

char	*retrieve_var_name_fhd(char *p, t_expand_data *xdat)
{
	int		i;
	char	*name;

	if (!p || p[0] != '$' || !xdat)
		return (NULL);
	if (p[1] == '?')
		return (ft_strdup("?"));
	i = 1;
	while (valid_name_char(p[i]))
		i++;
	if (i == 1 && p[i] == '"')
		return (NULL);
	name = (char *) ft_calloc(i, sizeof(char));
	if (!name)
		return (NULL);
	i = 0;
	while (valid_name_char(p[++i]))
		name[i - 1] = p[i];
	return (name);
}

int	retrieve_var_value_fhd(t_shell *sh, char *p, t_expand_data *xdat)
{
	char	*name;

	name = retrieve_var_name_fhd(p, xdat);
	if (!name)
		return (0);
	if (!xdat)
		return (-1);
	if (!ft_strcmp(name, "?"))
		xdat->tmp_val = ft_itoa(sh->exit_code);
	else
		xdat->tmp_val = ft_strdup(get_variable_value(sh, name));
	if (!ft_strcmp(name, "~"))
		xdat->name_size = ft_strlen(name);
	else
		xdat->name_size = ft_strlen(name) + 1;
	free(name);
	return (1);
}

int	get_new_size_fhd(t_shell *sh, char *word)
{
	int				i;
	int				new_size;
	t_expand_data	xdat;

	i = 0;
	new_size = ft_strlen(word);
	while (word[i])
	{
		if (retrieve_var_value_fhd(sh, &word[i], &xdat) > 0)
		{
			new_size -= xdat.name_size;
			new_size += ft_strlen(xdat.tmp_val);
			i += xdat.name_size;
			free(xdat.tmp_val);
		}
		else
			i++;
	}
	return (new_size);
}

char	*expand_fhd(t_shell *sh, char *word)
{
	int				i;
	int				j;
	t_expand_data	xdat;
	char			*new_word;

	i = 0;
	j = 0;
	xdat.new_size = get_new_size_fhd(sh, word);
	new_word = (char *) ft_calloc(xdat.new_size + 1, 1UL);
	if (!new_word)
		return (NULL);
	while (word[i])
	{
		if (retrieve_var_value_fhd(sh, &word[i], &xdat) > 0)
		{
			if (xdat.tmp_val)
				ft_strcpy(&new_word[j], xdat.tmp_val);
			i += xdat.name_size;
			j += ft_strlen(xdat.tmp_val);
			free(xdat.tmp_val);
		}
		else
			new_word[j++] = word[i++];
	}
	return (new_word);
}
