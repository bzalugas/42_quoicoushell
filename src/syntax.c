/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:37:34 by jsommet           #+#    #+#             */
/*   Updated: 2024/09/12 15:16:30 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

//either 313131313 or 33333 when i want 31333313111333

int	count_total_tokens(char *s)
{
	int		i;
	int		tc;

	i = 0;
	tc = 0;
	while (s[i])
	{
		if (ft_isoper(s[i]) || s[i] == '|')
		{
			tc++;
			i += (s[i + 1] && ft_isoper(s[i + 1]) && s[i] != '|');
		}
		else if (i == 0 && !ft_isspace(s[i]))
			tc++;
		if (s[i + 1] && (ft_isspace(s[i]) || ft_isoper(s[i]) || s[i] == '|')
			&& !ft_isspace(s[i + 1]) && !ft_isoper(s[i + 1]))// && s[i] == '|')
			tc++;
		if (ft_isquot(s[i]))
			i += next_quote(&s[i]);
		i++;
	}
	return (tc);
}

char	*create_syntax_line(char *l)
{
	int		i;
	int		j;
	char	*syntax_line;

	syntax_line = malloc((count_total_tokens(l) + 1) * sizeof(char));
	i = 0;
	j = 0;
	while (l[i])
	{
		if (ft_isoper(l[i]) || l[i] == '|')
		{
			syntax_line[j++] = 2 + (l[i] == '|');
			i += (l[i + 1] && ft_isoper(l[i + 1]) && l[i] != '|');
		}
		else if (i == 0 && !ft_isspace(l[i]))
			syntax_line[j++] = 1;
		if (l[i + 1] && (ft_isspace(l[i]) || ft_isoper(l[i]) || l[i] == '|')
			&& !ft_isspace(l[i + 1]) && !ft_isoper(l[i + 1]))// && l[i] != '|')
			syntax_line[j++] = 1;
		if (ft_isquot(l[i]))
			i += next_quote(&l[i]);
		i++;
	}
	return (syntax_line);
}

int	has_unmatched_quote(char *line)
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

void	set_expected(int *expected, int a, int b, int c)
{
	expected[0] = a;
	expected[1] = b;
	expected[2] = c;
}

void	yesp(char *s, int fd, int y)
{
	size_t	i;
	char	c;
	if (!s)
		return ;
	i = 0;
	while (s[i])
	{
		c = s[i++] + y;
		write(fd, &c, 1);
	}
	write(fd, "\n", 1);
}


int	check_syntax(char *line)
{
	char	*syntax_line;
	int		expected[4];
	int		i;

	syntax_line = create_syntax_line(line);
	yesp(syntax_line, 2, 48); //
	i = 0;
	while (syntax_line[i])
	{
		ft_bzero(expected, 4 * sizeof(int));
		if (!expected[syntax_line[i] - 1]
			|| (syntax_line[i] == 3 && i == 0))
		{
			free(syntax_line);
			return (0);
		}
		if (syntax_line[i] == 1)
			set_expected(expected, 1, 1, 1);
		else if (syntax_line[i] == 2)
			set_expected(expected, 1, 0, 0);
		else if (syntax_line[i] == 3)
			set_expected(expected, 1, 1, 0);
		i++;
	}
	free(syntax_line);
	return (expected[2] && !has_unmatched_quote(line));
}
