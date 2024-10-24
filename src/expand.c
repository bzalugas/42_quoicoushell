/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 19:10:07 by jsommet           #+#    #+#             */
/*   Updated: 2024/10/04 13:16:28 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

void	expand(t_shell *sh, char *word, t_expand_data *xdat)
{
	int		quote_size;

	(void) quote_size;
	bzero(&xdat->i, sizeof(int) * 2);
	xdat->new_word = (char *) ft_calloc(xdat->new_size + 1, 1UL);
	if (!xdat->new_word)
		return ;
	while (word[xdat->i])
	{
		while (active_quote(word, xdat->i) == C_SQ)
			xdat->new_word[xdat->j++] = word[xdat->i++];
		if (retrieve_var_value(sh, &word[xdat->i], xdat) > 0)
		{
			if (xdat->tmp_val)
				ft_strcpy(&(xdat->new_word[xdat->j]), xdat->tmp_val);
			xdat->i += xdat->name_size;
			xdat->j += ft_strlen(xdat->tmp_val);
			free(xdat->tmp_val);
		}
		else
			xdat->new_word[xdat->j++] = word[xdat->i++];
	}
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
	while (word && word[++i])
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

	xdat.full_str = word;
	xdat.new_size = get_new_size(sh, word, &xdat);
	replace_quotes(word);
	expand(sh, word, &xdat);
	if (!xdat.new_word)
		exit_shell(sh, EXIT_FAILURE, false);
	remove_weird_quotes(xdat.new_word);
	return (xdat.new_word);
}
