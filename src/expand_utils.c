/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 19:11:26 by jsommet           #+#    #+#             */
/*   Updated: 2024/09/30 17:50:52 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	valid_name_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	*retrieve_var_name(char *p, t_expand_data *xdat)
{
	int		i;
	char	*name;

	if (p && p[0] == '~' && (!p[1] || ft_isspace(p[1])))
		return (ft_strdup("~"));
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

int	retrieve_var_value(t_shell *sh, char *p, t_expand_data *xdat)
{
	char	*name;

	name = retrieve_var_name(p, xdat);
	if (!name)
		return (0);
	if (!xdat)
		return (-1);
	if (!ft_strcmp(name, "?"))
		xdat->tmp_val = ft_itoa(sh->exit_code);
	else if (!ft_strcmp(name, "~"))
		xdat->tmp_val = ft_strdup(get_variable_value(sh, "HOME"));
	else
		xdat->tmp_val = ft_strdup(get_variable_value(sh, name));
	if (!ft_strcmp(name, "~"))
		xdat->name_size = ft_strlen(name);
	else
		xdat->name_size = ft_strlen(name) + 1;
	free(name);
	return (1);
}

int	get_new_size(t_shell *sh, char *word, t_expand_data *xdat)
{
	int		i;
	int		new_size;

	i = 0;
	new_size = ft_strlen(word);
	while (word[i])
	{
		if (word[i] == '\'')
			i += next_quote(&word[i]);
		if (retrieve_var_value(sh, &word[i], xdat) > 0)
		{
			new_size -= xdat->name_size;
			new_size += ft_strlen(xdat->tmp_val);
			free(xdat->tmp_val);
			i += xdat->name_size;
		}
		else
			i++;
	}
	return (new_size);
}

int	split_expand_count(t_shell *sh, char *word)
{
	int				wc;
	t_expand_data	xdat;

	word = ft_strdup(word);
	if (!word)
		exit_shell(sh, EXIT_FAILURE, false);
	xdat.new_size = get_new_size(sh, word, &xdat);
	replace_quotes(word);
	expand(sh, word, &xdat);
	free(word);
	word = xdat.new_word;
	if (!word)
		return (0);
	replace_wsp(word);
	remove_weird_quotes(word);
	wc = count_words(word, C_WSP);
	free(word);
	return (wc);
}
