/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 15:36:58 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/07 03:08:08 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	ft_isoper(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	ft_isquot(char c)
{
	return (c == '\"' || c == '\'');
}

/*
* @return distance from this quote to the next one, 0 if not matched,
* -1 if p is NULL or if *p is not a quote.
* @param p Pointer to the quote's position.
*/
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

int	count_tokens(char *s)
{
	int		i;
	int		tc;

	i = 0;
	tc = 0;
	while (s[i])
	{
		while (s[i] && ft_isspace(s[i]))
			i++;
		if (s[i] && !ft_isspace(s[i]))
			tc++;
		while (s[i] && !ft_isspace(s[i]))
		{
			if (ft_isoper(s[i]))
			{
				tc++;
				if (s[++i] && ft_isoper(s[i]))
					i++;
			}
			if (s[i] && ft_isquot(s[i]))
				i += next_quote(&s[i]);
			i++;
		}
	}
	return (tc);
}

int	token_len(char *p)
{
	int	i;

	if (!p || !*p)
		return (0);
	i = 0;
	if (ft_isoper(p[i]))
		return (1 + ft_isoper(p[i + 1]));
	while (p[i] && !ft_isspace(p[i]) && !ft_isoper(p[i]))
	{
		if (p[i] && ft_isquot(p[i]))
			i += next_quote(&p[i]);
		i++;
	}
	return (i);
}

char	**token_split(char *s)
{
	char	**split;
	int		tc;
	int		tl;
	int		i;

	tc = count_tokens(s);
	split = (char **) calloc(tc + 1, sizeof(char *));
	i = 0;
	while (i < tc)
	{
		while (ft_isspace(*s))
			s++;
		tl = token_len(s);
		split[i] = (char *) calloc(tl + 1, sizeof(char));
		ft_strlcpy(split[i], s, tl + 1);
		s += tl;
		i++;
	}
	return (split);
}
