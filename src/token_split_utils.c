/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_split_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 19:13:43 by jsommet           #+#    #+#             */
/*   Updated: 2024/09/12 17:34:01 by jsommet          ###   ########.fr       */
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

	//TODO: RE WRITE POST PROCESSING
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

//returns distance to corresponding closing parenthesis
//needs to skip quotes
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
