/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 15:36:58 by jsommet           #+#    #+#             */
/*   Updated: 2024/09/17 19:11:44 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	count_tokens(char *s, t_tokens *t)
{
	int		i;
	int		tc;

	i = 0;
	tc = 0;
	while (s[i] && s[i] != '|')
	{
		if (ft_isoper(s[i]))
		{
			tc++;
			i += (s[i + 1] && ft_isoper(s[i + 1]));
		}
		else if (i == 0 && !ft_isspace(s[i]))
			tc++;
		if (s[i + 1] && s[i + 1] != '|' && (ft_isspace(s[i]) || ft_isoper(s[i]))
			&& !ft_isspace(s[i + 1]) && !ft_isoper(s[i + 1]))
			tc++;
		if (ft_isquot(s[i]))
			i += next_quote(&s[i]);
		i++;
	}
	t->start += i + 1;
	if (s[i] == '\0')
		t->start = -1;
	return (tc);
}

int	token_len(char *p)
{
	int	i;

	if (!p || !*p)
		return (0);
	if (ft_isoper(p[0]))
	{
		if (ft_isoper(p[1]))
			return (2);
		return (1);
	}
	i = 0;
	while (p[i] && p[i] != '|' && !ft_isspace(p[i]) && !ft_isoper(p[i]))
	{
		if (p[i] && ft_isquot(p[i]))
			i += next_quote(&p[i]);
		i++;
	}
	return (i);
}

char	**token_split(char *s, t_tokens *t)
{
	char		**split;
	int			tc;
	int			tl;
	int			i;

	t->cmd_n++;
	s = &s[t->start];
	tc = count_tokens(s, t);
	split = (char **) ft_calloc(tc + 1, sizeof(char *));
	i = -1;
	while (++i < tc)
	{
		while (ft_isspace(*s))
			s++;
		tl = token_len(s);
		split[i] = (char *) ft_calloc(tl + 1, sizeof(char));
		ft_strlcpy(split[i], s, tl + 1);
		s += tl;
	}
	return (split);
}
