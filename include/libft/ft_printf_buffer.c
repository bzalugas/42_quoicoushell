/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_buffer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 13:22:10 by bazaluga          #+#    #+#             */
/*   Updated: 2024/06/22 13:26:50 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

t_buffer	*buff_init(void)
{
	t_buffer	*buf;

	buf = (t_buffer *)malloc(sizeof(t_buffer));
	if (!buf)
		return (NULL);
	buf->tot_len = 0;
	buf->first = NULL;
	return (buf);
}

bool	buff_add_back(t_buffer *buf, t_node *new)
{
	t_node	*tmp;

	if (!new || !buf)
		return (false);
	if (!buf->first)
		buf->first = new;
	else
	{
		tmp = buf->first;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	buf->tot_len += new->len;
	return (true);
}

bool	buff_add_after(t_buffer *buf, t_node *node, t_node *new)
{
	if (!node || !new)
		return (false);
	new->next = node->next;
	node->next = new;
	buf->tot_len += new->len;
	return (true);
}

bool	buff_add_before(t_buffer *buf, t_node *node, t_node *new)
{
	t_node	*tmp;

	if (!buf || !node || !new)
		return (false);
	new->next = node;
	if (buf->first == node)
		buf->first = new;
	else
	{
		tmp = buf->first;
		while (tmp && tmp->next != node)
			tmp = tmp->next;
		if (!tmp)
			return (false);
		tmp->next = new;
	}
	buf->tot_len += new->len;
	return (true);
}
