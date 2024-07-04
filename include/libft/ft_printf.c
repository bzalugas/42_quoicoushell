/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 23:39:50 by bazaluga          #+#    #+#             */
/*   Updated: 2024/06/22 13:26:28 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

static t_type	get_type(char c)
{
	if (c == 'c')
		return (CHAR);
	if (c == 's')
		return (STR);
	if (c == 'd' || c == 'i')
		return (INT);
	if (c == 'u')
		return (UINT);
	if (c == 'x')
		return (LHEX);
	if (c == 'X')
		return (UHEX);
	if (c == 'p')
		return (PTR);
	if (c == 'f')
		return (FLOAT);
	if (c == '%')
		return (PERCENT);
	return (ERR);
}

static bool	parse(t_buffer *buf, char *str, int *i, va_list args)
{
	t_flags	*flags;
	t_type	type;

	if (*i > 0)
	{
		str[*i] = '\0';
		if (!buff_add_back(buf, node_new(LIT, (*i)++, str)))
			return (false);
	}
	else
		++(*i);
	flags = flags_init();
	if (!flags_get(flags, str, i, args))
		return (false);
	type = get_type(str[*i]);
	if (type == ERR || !buff_add_back(buf, node_new(type, 0, flags)))
	{
		free(flags);
		return (false);
	}
	return (true);
}

static int	tokenize(t_buffer *buf, char *str, va_list args)
{
	int		i;
	bool	check;

	i = 0;
	while (str[i])
	{
		if (str[i] == '%')
		{
			check = parse(buf, str, &i, args);
			if (!check)
				return (-1);
			str += i + 1;
			i = -1;
		}
		i++;
	}
	if (i > 0)
		return (buff_add_back(buf, node_new(LIT, i, str)) - 1);
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list		args;
	t_buffer	*buf;
	char		*str;
	long		count;

	if (!format)
		return (-1);
	buf = buff_init();
	str = ft_strdup(format);
	if (!buf || !str)
		return (buff_clear(&buf), -1);
	va_start(args, format);
	if (tokenize(buf, str, args) == -1)
	{
		count = buff_print(buf);
		return (va_end(args), buff_clear(&buf), -1);
	}
	if (!convert_buffer(buf, args))
		return (va_end(args), buff_clear(&buf), -1);
	count = buff_print(buf);
	free(str);
	buff_clear(&buf);
	va_end(args);
	return ((int)count);
}
