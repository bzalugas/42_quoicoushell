/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 13:24:00 by bazaluga          #+#    #+#             */
/*   Updated: 2024/06/22 13:27:47 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

/* Helper for int/uint conversions */

bool	get_float_adds(char **sp, t_flags *f)
{
	if (f->width)
		*sp = (char *)ft_calloc(f->width + 1, sizeof(char));
	if (!*sp && f->width)
		return (free(*sp), false);
	if (*sp && !f->zero)
		ft_memset(*sp, ' ', f->width);
	else if (*sp && f->zero)
		ft_memset(*sp, '0', f->width);
	return (true);
}

bool	get_int_uint_adds(char **sp, char **zer, t_flags *f)
{
	if (f->width)
		*sp = (char *)ft_calloc(f->width + 1, sizeof(char));
	if (f->pad)
		*zer = (char *)ft_calloc(f->pad + 1, sizeof(char));
	if (!*sp && f->width && *zer)
	{
		free(*zer);
		return (false);
	}
	if (*sp && !*zer && f->pad)
	{
		free(*sp);
		return (false);
	}
	if (!*sp && !*zer && f->width && f->pad)
		return (false);
	if (*sp && !f->zero)
		ft_memset(*sp, ' ', f->width);
	if (*sp && f->zero)
		ft_memset(*sp, '0', f->width);
	if (*zer)
		ft_memset(*zer, '0', f->pad);
	return (true);
}

/* Helper for hex conversions */

bool	get_hex_adds(char **sp, char **zer, t_flags *f)
{
	if (f->width)
		*sp = (char *)ft_calloc(f->width + 1, sizeof(char));
	if (f->pad)
		*zer = (char *)ft_calloc(f->pad + 1, sizeof(char));
	if (!*sp && f->width && *zer)
	{
		free(*zer);
		return (false);
	}
	if (*sp && !*zer && f->pad)
	{
		free(*sp);
		return (false);
	}
	if (!*sp && !*zer && f->width && f->pad)
		return (false);
	if (*sp && !f->zero)
		ft_memset(*sp, ' ', f->width);
	if (*sp && f->zero)
		ft_memset(*sp, '0', f->width);
	if (*zer)
		ft_memset(*zer, '0', f->pad);
	return (true);
}
