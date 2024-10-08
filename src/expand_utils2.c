/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 12:47:30 by jsommet           #+#    #+#             */
/*   Updated: 2024/10/04 13:16:41 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	valid_name_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	active_quote(char *s, int index)
{
	char	q;
	int		i;

	q = 0;
	i = 0;
	while (s[i])
	{
		if (i == index)
			return (q);
		if (!q && ft_isquot(s[i]))
			q = s[i];
		else if (ft_isquot(q) && q == s[i])
			q = 0;
		i++;
	}
	return (0);
}

bool	is_valid_tilde(char *p, t_expand_data *xdat)
{
	int		i;

	i = (int)(p - xdat->full_str);
	return (p && (i == 0 || ft_isspace(xdat->full_str[i - 1])) && p[0] == '~'
		&& (!p[1] || p[1] == '/' || ft_isspace(p[1]))
		&& !active_quote(xdat->full_str, i));
}
