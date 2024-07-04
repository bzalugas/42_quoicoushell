/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_flags_parsing.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 13:23:41 by bazaluga          #+#    #+#             */
/*   Updated: 2024/06/22 13:27:31 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

t_flags	*flags_init(void)
{
	t_flags	*new;

	new = (t_flags *)malloc(sizeof(t_flags));
	if (!new)
		return (NULL);
	ft_memset(new, 0, sizeof(t_flags));
	return (new);
}

static int	flags_handle_star(t_flags *f, char *str, int *i, va_list args)
{
	va_list		tmp;
	int			nb;
	int			n_arg;

	(*i)++;
	n_arg = ft_atoi(str);
	f->n_star = n_arg + (n_arg == 0);
	while (str[*i] && ft_isdigit(str[*i]))
		(*i)++;
	if (!str[*i] || (f->n_star > 1 && str[*i] != '$'))
		return (-1);
	va_copy(tmp, args);
	nb = -1;
	if (n_arg == 0 || n_arg == 1)
		nb = va_arg(tmp, int);
	else
		while (--n_arg > 0)
			nb = va_arg(tmp, int);
	va_end(tmp);
	return (nb);
}

static void	flags_from_char(t_flags *flags, char c)
{
	flags->minus |= c == '-';
	flags->zero |= c == '0';
	flags->zero &= !flags->minus;
	flags->dot |= c == '.';
	flags->sharp |= c == '#';
	flags->space |= c == ' ';
	flags->plus |= c == '+';
}

bool	flags_get(t_flags *flags, char *str, int *i, va_list args)
{
	if (!flags || !str[*i])
		return (false);
	while (str[*i] && (ft_strchr(FLAGS, str[*i]) || ft_isdigit(str[*i])))
	{
		flags_from_char(flags, str[*i]);
		if (str[*i] == '*' && flags->dot)
			flags->pad = flags_handle_star(flags, str, i, args);
		else if (str[*i] == '*')
			flags->width = flags_handle_star(flags, str, i, args);
		else if (ft_isdigit(str[*i]) && str[*i] != '0')
		{
			if (flags->dot)
				flags->pad = ft_atol(&str[*i]);
			else
				flags->width = ft_atol(&str[*i]);
			if (flags->pad > INT_MAX || flags->width > INT_MAX)
				return (false);
			while (str[*i] && ft_isdigit(str[*i]))
				(*i)++;
		}
		else
			(*i)++;
	}
	return (true);
}
