/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 12:36:33 by bazaluga          #+#    #+#             */
/*   Updated: 2024/02/23 20:31:56 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_find_nl(const char *s, char **nl)
{
	size_t	i;
	int		c;

	if (!s || !nl)
		return (-1);
	c = '\n';
	i = 0;
	while (s[i])
	{
		if ((unsigned char)c == (unsigned char)s[i])
			return (*nl = (char *)&s[i], 1);
		i++;
	}
	*nl = (char *)&s[i];
	if ((unsigned char)c == (unsigned char)s[i])
		return (1);
	return (0);
}

void	*ft_calloc_gnl(char **dst, size_t nmemb, size_t size)
{
	unsigned char	*ptr;
	size_t			i;

	if (size > 0 && nmemb > ULONG_MAX / size)
		return (NULL);
	ptr = (unsigned char *)malloc(nmemb * size);
	if (dst)
		*dst = (char *)ptr;
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < nmemb * size)
	{
		ptr[i] = '\0';
		i++;
	}
	return ((void *)ptr);
}
