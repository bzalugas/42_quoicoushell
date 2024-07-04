/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_float_handlers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 13:23:51 by bazaluga          #+#    #+#             */
/*   Updated: 2024/06/22 20:59:29 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

static bool	float_put_add(t_buffer *buf, t_node *node, bool neg)
{
	char	*sp;
	t_flags	*f;

	sp = NULL;
	f = (t_flags *)node->content;
	if (!get_float_adds(&sp, f))
		return (false);
	if (f->width && f->zero && neg)
		sp[0] = '-';
	if (f->width && f->zero && f->plus && !neg)
		sp[0] = '+';
	if (f->width && f->space && !neg)
		sp[0] = ' ';
	if (f->minus && !buff_add_after(buf, node, node_new(CONV, f->width, sp)))
		return (false);
	else if (!f->minus && f->width && !buff_add_before(buf, node,
			node_new(CONV, f->width, sp)))
		return (false);
	return (true);
}

static bool	handle_flags_float(t_buffer *buf, t_node *node, t_flags *f, char *n)
{
	int	len_n;

	len_n = ft_strlen(n);
	f->zero &= (!f->minus && (f->width > len_n));
	f->minus &= f->width > len_n;
	f->space &= n[0] != '-';
	f->plus &= n[0] != '-';
	f->width = (f->width - (len_n + f->space));
	f->width = (f->width > 0) * f->width;
	f->width += (f->space && !f->plus);
	return (float_put_add(buf, node, n[0] == '-'));
}

bool	handle_float(t_buffer *buf, t_node *node, float arg)
{
	char	*n;
	t_flags	*f;

	n = NULL;
	f = (t_flags *)node->content;
	if (!f->dot)
		f->pad = 6;
	n = ft_ftoa_printf(arg, f->pad, f);
	if (!n)
		return (false);
	if (!handle_flags_float(buf, node, f, n))
		return (false);
	if ((arg < 0 || f->space) && f->zero)
		n[0] = '0';
	free(node->content);
	node->content = n;
	node->len = ft_strlen(n);
	buf->tot_len += node->len;
	return (true);
}
