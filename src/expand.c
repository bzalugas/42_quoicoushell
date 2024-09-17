/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 19:10:07 by jsommet           #+#    #+#             */
/*   Updated: 2024/09/17 18:51:27 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

char	*expand(t_shell *sh, char *word, t_expand_data *xdat)
{
	int		i;
	int		j;
	int		quote_size;
	char	*new_word;

	i = 0;
	j = 0;
	new_word = (char *) ft_calloc(xdat->new_size + 1, 1UL);
	while (word[i])
	{
		if (word[i] == C_SQ)
		{
			quote_size = next_quote(&word[i]);
			while (--quote_size > 0)
				new_word[j++] = word[i++];
		}
		if (retrieve_var_value(sh, &word[i], xdat) > 0)
		{
			if (xdat->tmp_val)
				ft_strcpy(&new_word[j], xdat->tmp_val);
			i += xdat->name_size + 1;
			j += ft_strlen(xdat->tmp_val);
			free(xdat->tmp_val);
		}
		else
			new_word[j++] = word[i++];
	}
	return (new_word);
}

void	replace_quotes(char *word)
{
	int	i;
	int	nq;

	i = 0;
	while (word[i])
	{
		nq = next_quote(&word[i]);
		if (nq > 0)
		{
			if (word[i] == '\'')
				word[i] = C_SQ;
			if (word[i] == '"')
				word[i] = C_DQ;
			i += nq;
			if (word[i] == '\'')
				word[i] = C_SQ;
			if (word[i] == '"')
				word[i] = C_DQ;
		}
		i++;
	}
}

void	replace_wsp(char *word)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (word[i])
	{
		if (!quote && (word[i] == C_SQ || word[i] == C_DQ))
			quote = word[i];
		else if (quote == word[i])
			quote = 0;
		if (!quote && ft_isspace(word[i]))
			word[i] = C_WSP;
		i++;
	}
}

void	remove_weird_quotes(char *word)
{
	int		offset;
	int		i;
	int		q;
	char	c;

	offset = 0;
	i = -1;
	c = 0;
	while (word[++i])
	{
		word[i - offset] = word[i];
		q = (word[i] == C_SQ || word[i] == C_DQ);
		if (c && c == word[i])
		{
			offset++;
			c = 0;
		}
		else if (!c && q > 0)
		{
			offset++;
			c = word[i];
		}
	}
	while (word[i - offset])
		word[i++ - offset] = 0;
}

char	*remove_quotes_and_expand(t_shell *sh, char *word)
{
	t_expand_data	xdat;

	xdat.new_size = get_new_size(sh, word, &xdat);
	replace_quotes(word);
	word = expand(sh, word, &xdat);
	remove_weird_quotes(word);
	return (word);
}
