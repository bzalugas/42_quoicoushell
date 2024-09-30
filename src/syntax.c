/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:37:34 by jsommet           #+#    #+#             */
/*   Updated: 2024/09/27 14:50:50 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"
//either 313131313 or 33333 when i want 31333313111333

int	count_total_tokens(char *l)
{
	int		i;
	int		tc;

	i = 0;
	tc = 0;
	while (l[i])
	{
		while (ft_isspace(l[i]))
			i++;
		if (l[i] && !ft_isoper(l[i]) && l[i] != '|' && !ft_isspace(l[i]))
			tc++;
		while (l[i] && !ft_isoper(l[i]) && l[i] != '|' && !ft_isspace(l[i]))
			i += increment(&l[i]);
		if (ft_isoper(l[i]))
		{
			tc++;
			i += 1 + (l[i + 1] == l[i]);
		}
		if (l[i] == '|')
		{
			tc++;
			i++;
		}
	}
	return (tc);
}

char	*create_syntax_line(char *l)
{
	int		i[2];
	char	*syntax_line;

	syntax_line = calloc(count_total_tokens(l) + 1, sizeof(char));
	ft_bzero(i, sizeof(int) * 2);
	while (syntax_line && l[i[0]])
	{
		while (ft_isspace(l[i[0]]))
			i[0]++;
		if (l[*i] && !ft_isoper(l[*i]) && l[*i] != '|' && !ft_isspace(l[*i]))
			syntax_line[i[1]++] = 1;
		while (l[*i] && !ft_isoper(l[*i]) && l[*i] != '|' && !ft_isspace(l[*i]))
			i[0] += increment(&l[i[0]]);
		if (ft_isoper(l[i[0]]))
		{
			syntax_line[i[1]++] = 2;
			i[0] += 1 + (l[i[0] + 1] == l[i[0]]);
		}
		if (l[i[0]] == '|')
		{
			syntax_line[i[1]++] = 3;
			i[0]++;
		}
	}
	return (syntax_line);
}

int	check_syntax(char *line)
{
	char	*syntax_line;
	int		expected[4];
	int		i;

	syntax_line = create_syntax_line(line);
	i = 0;
	set_expected(expected, 1, 1, 0);
	while (syntax_line && syntax_line[i])
	{
		if (!expected[syntax_line[i] - 1])
		{
			free(syntax_line);
			return (0);
		}
		ft_bzero(expected, 4 * sizeof(int));
		if (syntax_line[i] == 1)
			set_expected(expected, 1, 1, 1);
		else if (syntax_line[i] == 2)
			set_expected(expected, 1, 0, 0);
		else if (syntax_line[i] == 3)
			set_expected(expected, 1, 1, 0);
		i++;
	}
	free(syntax_line);
	return ((expected[2] || i == 0) && !has_open_quote(line));
}
