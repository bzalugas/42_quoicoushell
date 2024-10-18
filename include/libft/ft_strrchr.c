/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 12:18:09 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/18 18:56:52 by jsommet          ###   ########.fr       */
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
