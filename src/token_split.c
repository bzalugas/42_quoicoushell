/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 15:36:58 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/08 01:14:57 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"
/*
* @return distance from this quote to the next one, 0 if not matched,
* -1 if p is NULL or if *p is not a quote.
* @param p Pointer to the quote's position.
*/

/****** WITH PARENTHESIS (somewhat BROKEN ( "(()))))" reads as "(())" )) ******/
/*
int	count_tokens(char *s)
{
	int		i;
	int		tc;

	i = 0;
	tc = 0;
	while (s[i])
	{
		if (ft_isoper(s[i]) && i > 0)
		{
			tc++;
			i += (s[i + 1] && s[i] == s[i + 1]);
		}
		if ((i == 0 && !ft_isspace(s[i]))
			|| (i > 0 && ((ft_isspace(s[i - 1]) || ft_isoper(s[i - 1]))
					&& !ft_isspace(s[i]) && !ft_isoper(s[i]))))
			tc++;
		if (ft_isquot(s[i]))
			i += next_quote(&s[i]);
		if (s[i] == '(')
			i += close_par(&s[i]);
		i++;
	}
	return (tc);
}

int	token_len(char *p)
{
	int	i;

	if (!p || !*p)
		return (0);
	if (ft_isoper(p[0]))
	{
		if (p[0] == p[1])
			return (2);
		return (1);
	}
	i = close_par(p);
	if (i < 0)
		i = 0;
	while (p[i] && !ft_isspace(p[i]) && !ft_isoper(p[i]))
	{
		if (p[i] && ft_isquot(p[i]))
			i += next_quote(&p[i]);
		i++;
	}
	return (i);
}
*/

/****** NO PARENTHESIS ******/
int	count_tokens(char *s)
{
	int		i;
	int		tc;

	i = 0;
	tc = 0;
	while (s[i])
	{
		if (ft_isoper(s[i]))
		{
			tc++;
			i += (s[i + 1] && s[i] == s[i + 1]);
		}
		else if (i == 0 && !ft_isspace(s[i]))
			tc++;
		if (s[i + 1] && (ft_isspace(s[i]) || ft_isoper(s[i]))
			&& !ft_isspace(s[i + 1]) && !ft_isoper(s[i + 1]))
			tc++;
		if (ft_isquot(s[i]))
			i += next_quote(&s[i]);
		i++;
	}
	return (tc);
}

int	token_len(char *p)
{
	int	i;

	if (!p || !*p)
		return (0);
	if (ft_isoper(p[0]))
	{
		if (p[0] == p[1])
			return (2);
		return (1);
	}
	i = 0;
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
	ft_putnbr_fd(tc, 2);
	ft_putchar_fd('\n', 2);
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
