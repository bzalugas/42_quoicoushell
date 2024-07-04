/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 14:04:08 by bazaluga          #+#    #+#             */
/*   Updated: 2024/06/07 12:39:16 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_lstadd_front(t_list **lst, t_list *new)
{
	t_list	*old;

	if (!lst || !new)
		return (NULL);
	old = *lst;
	*lst = new;
	if (new)
		new->next = old;
	return (new);
}
