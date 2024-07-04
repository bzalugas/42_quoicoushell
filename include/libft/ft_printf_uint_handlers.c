/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_uint_handlers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 13:24:34 by bazaluga          #+#    #+#             */
/*   Updated: 2024/06/22 13:28:26 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

static bool	uint_put_add(t_buffer *buf, t_node *node, t_flags *f)
{
	char	*sp;
	char	*zer;

	sp = NULL;
	zer = NULL;
	if (!get_int_uint_adds(&sp, &zer, f))
		return (false);
	if (f->minus && !buff_add_after(buf, node, node_new(CONV, f->width, sp)))
		return (false);
	if (!f->minus && f->width && !buff_add_before(buf, node,
			node_new(CONV, f->width, sp)))
		return (false);
	if (f->pad && !buff_add_before(buf, node, node_new(CONV, f->pad, zer)))
		return (false);
	return (true);
}

static bool	handle_flags_uint(t_buffer *buf, t_node *node, t_flags *f, int len)
{
	f->zero &= (!f->dot && (f->width > len));
	f->dot &= f->pad > len;
	f->minus &= f->width > len;
	f->pad = f->pad - len;
	f->pad = (f->pad > 0) * f->pad;
	f->width = (f->width - len - f->pad);
	f->width = (f->width > 0) * f->width;
	return (uint_put_add(buf, node, f));
}

bool	handle_uint(t_buffer *buf, t_node *node, unsigned int arg)
{
	char	*n;
	t_flags	*f;
	int		len;

	f = (t_flags *)node->content;
	n = ft_utoa_printf(arg);
	if (!n)
		return (false);
	if (f->dot && f->pad == 0 && arg == 0)
		n[0] = '\0';
	len = ft_strlen(n);
	if (!handle_flags_uint(buf, node, f, len))
		return (false);
	free(node->content);
	node->content = n;
	node->len = ft_strlen(n);
	buf->tot_len += node->len;
	return (true);
}
