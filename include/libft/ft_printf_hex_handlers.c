/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_hex_handlers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 13:24:08 by bazaluga          #+#    #+#             */
/*   Updated: 2024/06/22 13:27:56 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

static bool	hex_put_add(t_buffer *buf, t_node *node, t_flags *f, char ox[3])
{
	char	*sp;
	char	*zer;

	sp = NULL;
	zer = NULL;
	if (!get_int_uint_adds(&sp, &zer, f))
		return (false);
	if (f->minus && !buff_add_after(buf, node, node_new(CONV, f->width, sp)))
		return (false);
	if (f->sharp && (f->minus || f->zero) && !buff_add_before(buf, node,
			node_new(CONV, 2, ft_strdup(ox))))
		return (false);
	if (!f->minus && f->width && !buff_add_before(buf, node,
			node_new(CONV, f->width, sp)))
		return (false);
	if (f->sharp && !f->minus && !f->zero && !buff_add_before(buf, node,
			node_new(CONV, 2, ft_strdup(ox))))
		return (false);
	if (f->pad && !buff_add_before(buf, node, node_new(CONV, f->pad, zer)))
		return (false);
	return (true);
}

static bool	handle_flags_hex(t_buffer *buf, t_node *node, t_flags *f, int len)
{
	char	ox[3];

	f->zero &= (!f->dot && (f->width > len));
	f->dot &= f->pad > len;
	f->minus &= f->width > len;
	f->pad = f->pad - len;
	f->pad = (f->pad > 0) * f->pad;
	f->width = (f->width - len - f->pad) - (f->sharp * 2);
	f->width = (f->width > 0) * f->width;
	ox[0] = '0';
	ox[2] = '\0';
	if (node->type == LHEX)
		ox[1] = 'x';
	else
		ox[1] = 'X';
	return (hex_put_add(buf, node, f, ox));
}

bool	handle_hex(t_buffer *buf, t_node *node, unsigned int arg)
{
	char	*n;
	t_flags	*f;
	int		len;

	f = (t_flags *)node->content;
	n = ft_utohex_printf(arg, node->type == LHEX);
	if (!n)
		return (false);
	if (f->dot && f->pad == 0 && arg == 0)
		n[0] = '\0';
	len = ft_strlen(n);
	f->sharp &= arg > 0;
	if (!handle_flags_hex(buf, node, f, len))
		return (false);
	free(node->content);
	node->content = n;
	node->len = len;
	buf->tot_len += node->len;
	return (true);
}

static bool	handle_flags_ptr(t_buffer *buf, t_node *node, t_flags *f, int len)
{
	f->zero = 0;
	f->dot = 0;
	f->minus &= f->width > len;
	f->pad = 0;
	f->width = (f->width - len) - (f->sharp * 2);
	f->width *= (f->width > 0);
	return (hex_put_add(buf, node, f, "0x"));
}

bool	handle_ptr(t_buffer *buf, t_node *node, void *ptr)
{
	char	*n;
	t_flags	*f;
	int		len;

	f = (t_flags *)node->content;
	if (!ptr)
	{
		n = ft_strdup("(nil)");
		f->sharp = 0;
	}
	else
	{
		n = ft_utohex_printf((unsigned long)ptr, 1);
		f->sharp = 1;
	}
	if (!n)
		return (false);
	len = ft_strlen(n);
	if (!handle_flags_ptr(buf, node, f, len))
		return (false);
	free(node->content);
	node->content = n;
	node->len = len;
	buf->tot_len += node->len;
	return (true);
}
