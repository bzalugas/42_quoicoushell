/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_split_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 19:13:43 by jsommet           #+#    #+#             */
/*   Updated: 2024/10/14 18:11:43 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	ft_isoper(char c)
{
	return (c == '<' || c == '>');
}

int	ft_isquot(char c)
{
	return (c == '\"' || c == '\'' || c == C_SQ || c == C_DQ);
}

int	next_quote(char *p)
{
	int		i;
	char	q;

	if (!p || !ft_isquot(*p))
		return (-1);
	q = *p;
	i = 0;
	while (p[++i])
	{
		if (p[i] == q)
			return (i);
	}
	return (0);
}

void	remove_quotes(char *word)
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
		q = next_quote(&word[i]);
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
