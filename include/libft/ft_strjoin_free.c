/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 12:57:16 by bazaluga          #+#    #+#             */
/*   Updated: 2024/06/19 13:03:32 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_free(char const *s1, char const *s2, int free1, int free2)
{
	char	*new;
	size_t	i;
	size_t	j;

	if (!s1 && s2)
		return (ft_strdup_free(s2, free2));
	if (!s2 && s1)
		return (ft_strdup_free(s1, free1));
	if (!s1 && !s2)
		return (NULL);
	new = ft_calloc(ft_strlen(s1) + ft_strlen(s2) + 1, sizeof(char));
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[j])
		new[i++] = s1[j++];
	j = 0;
	while (s2[j])
		new[i++] = s2[j++];
	if (free1)
		free((char *)s1);
	if (free2)
		free((char *)s2);
	return (new);
}
