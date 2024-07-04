/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstremove.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:36:23 by bazaluga          #+#    #+#             */
/*   Updated: 2024/05/26 19:00:20 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstremove(t_list **lst, t_list *link, void (*del)(void *))
{
	t_list	*first;

	first = *lst;
	if (first == link)
	{
		*lst = (*lst)->next;
		ft_lstdelone(first, del);
		return ;
	}
	while (first && first->next && first->next != link)
		first = first->next;
	if (first->next && first->next == link)
	{
		first->next = link->next;
		ft_lstdelone(link, del);
	}
}
