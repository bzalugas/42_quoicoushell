/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_int_handlers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 13:24:18 by bazaluga          #+#    #+#             */
/*   Updated: 2024/06/22 13:28:19 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

static void	modif_adds(char *sp, char *zer, t_flags *f, bool neg)
{
	if (f->pad && neg)
		zer[0] = '-';
	else if (f->pad && f->plus)
		zer[0] = '+';
	else if (f->zero && neg)
		sp[0] = '-';
	else if (f->zero && f->plus)
		sp[0] = '+';
	else if (f->pad && f->space)
		zer[0] = ' ';
}

static bool	int_put_add(t_buffer *buf, t_node *node, bool neg)
{
	char	*sp;
	char	*zer;
	t_flags	*f;

	sp = NULL;
	zer = NULL;
	f = (t_flags *)node->content;
	if (!get_int_uint_adds(&sp, &zer, f))
		return (false);
	modif_adds(sp, zer, f, neg);
	if (f->minus && !buff_add_after(buf, node, node_new(CONV, f->width, sp)))
		return (false);
	if (!f->minus && f->width && !buff_add_before(buf, node,
			node_new(CONV, f->width, sp)))
		return (false);
	if (f->pad && !buff_add_before(buf, node, node_new(CONV, f->pad, zer)))
		return (false);
	return (true);
}

static bool	handle_flags_int(t_buffer *buf, t_node *node, t_flags *f, char *n)
{
	int		len_n;

	len_n = ft_strlen(n);
	f->zero &= (!f->dot && (f->width > len_n));
	f->dot &= f->pad > (len_n - (n[0] == '-'));
	f->minus &= f->width > len_n;
	f->space &= n[0] != '-';
	f->plus &= n[0] != '-';
	f->pad = f->pad - len_n + (n[0] == '-' || f->plus || f->space);
	f->pad = (f->pad > 0) * f->pad;
	f->width = (f->width - len_n - f->pad);
	f->width = (f->width > 0) * f->width;
	return (int_put_add(buf, node, n[0] == '-'));
}

bool	handle_int(t_buffer *buf, t_node *node, int arg)
{
	char	*n;
	t_flags	*f;

	n = NULL;
	f = (t_flags *)node->content;
	if (f->dot && f->pad == 0 && arg == 0)
	{
		if (!handle_flags_int(buf, node, f, ""))
			return (false);
	}
	else
	{
		n = ft_itoa_printf(arg, f);
		if (!n)
			return (false);
		if (!handle_flags_int(buf, node, f, n))
			return (false);
		if ((arg < 0 || f->plus || f->space) && (f->pad || f->zero))
			n[0] = '0';
	}
	free(node->content);
	node->content = n;
	node->len = ft_strlen(n);
	buf->tot_len += node->len;
	return (true);
}
