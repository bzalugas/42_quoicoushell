/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_convert_buffer.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 13:23:33 by bazaluga          #+#    #+#             */
/*   Updated: 2024/06/22 13:23:39 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static bool	convert_node(t_buffer *buf, t_node *node, va_list args)
{
	bool	check;

	if (((t_flags *)node->content)->n_star)
		va_arg(args, int);
	check = false;
	if (node->type == CHAR)
		check = handle_char(buf, node, va_arg(args, int));
	else if (node->type == STR)
		check = handle_str(buf, node, va_arg(args, const char *));
	else if (node->type == INT)
		check = handle_int(buf, node, va_arg(args, int));
	else if (node->type == UINT)
		check = handle_uint(buf, node, va_arg(args, unsigned int));
	else if (node->type == LHEX || node->type == UHEX)
		check = handle_hex(buf, node, va_arg(args, unsigned int));
	else if (node->type == PTR)
		check = handle_ptr(buf, node, va_arg(args, void *));
	else if (node->type == FLOAT)
		check = handle_float(buf, node, va_arg(args, double));
	else if (node->type == PERCENT)
		check = handle_percent(buf, node);
	node->type = CONV;
	return (check);
}

bool	convert_buffer(t_buffer *buf, va_list args)
{
	t_node	*node;

	node = node_get_next_conversion(buf->first);
	while (node)
	{
		if (!convert_node(buf, node, args))
			return (false);
		node = node_get_next_conversion(node);
	}
	return (true);
}
