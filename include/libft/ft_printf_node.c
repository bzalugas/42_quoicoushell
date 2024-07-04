/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 13:24:26 by bazaluga          #+#    #+#             */
/*   Updated: 2024/06/22 13:24:32 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

t_node	*node_new(t_type type, size_t len, void *content)
{
	t_node	*new;

	new = (t_node *)malloc(sizeof(t_node));
	if (!new)
		return (NULL);
	new->type = type;
	new->len = len;
	new->content = content;
	new->next = NULL;
	return (new);
}

t_node	*node_get_next_conversion(t_node *node)
{
	while (node && (node->type == LIT || node->type == CONV))
		node = node->next;
	return (node);
}
