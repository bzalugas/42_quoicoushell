/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 12:18:09 by bazaluga          #+#    #+#             */
/*   Updated: 2023/11/13 20:07:17 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;
	char	*last;

	last = NULL;
	i = 0;
	while (s[i])
	{
		if ((unsigned char)c == (unsigned char)s[i])
			last = (char *)&s[i];
		i++;
	}
	if ((unsigned char)c == (unsigned char)s[i])
		last = (char *)&s[i];
	return (last);
}
