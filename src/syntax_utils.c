/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 18:52:00 by jsommet           #+#    #+#             */
/*   Updated: 2024/09/15 19:34:22 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	increment(char *p)
{
	int	q;

	q = next_quote(p);
	if (q > 0)
		return (q);
	return (1);
}

void	set_expected(int *expected, int a, int b, int c)
{
	expected[0] = a;
	expected[1] = b;
	expected[2] = c;
}

int	has_open_quote(char *line)
{
	int	nq;

	while (*line)
	{
		nq = next_quote(line);
		if (nq == 0)
			return (1);
		if (nq > 0)
			line += nq;
		line++;
	}
	return (0);
}
