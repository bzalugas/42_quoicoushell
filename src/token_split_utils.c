/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_split_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 19:13:43 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/08 01:12:16 by jsommet          ###   ########.fr       */
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

//returns distance to corresponding closing parenthesis
//returns 0 if corresponding parenthesis not found
// int	close_par(char *p)
// {
// 	int		i;
// 	int		op;

// 	if (!p || *p != '(')
// 		return (-1);
// 	i = 0;
// 	op = 0;
// 	while (p[++i])
// 	{
// 		if (p[i] == '(')
// 			op++;
// 		else if (p[i] == ')')
// 		{
// 			if (!op)
// 				return (i);
// 			op--;
// 		}
// 	}
// 	return (0);
// }
