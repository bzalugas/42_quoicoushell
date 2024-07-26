/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstunlink.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 04:34:41 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/26 14:18:27 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstunlink(t_list **lst, t_list *link)
{
	t_list	*first;

	if (!lst || !*lst)
		return ;
	first = *lst;
	if (first == link)
	{
		*lst = (*lst)->next;
		link->next = NULL;
		return ;
	}
	while (first && first->next && first->next != link)
		first = first->next;
	if (first->next && first->next == link)
	{
		first->next = link->next;
	}
	link->next = NULL;
}
