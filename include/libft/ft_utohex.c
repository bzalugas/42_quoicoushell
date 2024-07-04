/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utohex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 17:41:06 by bazaluga          #+#    #+#             */
/*   Updated: 2024/02/23 17:41:46 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	len_uint_hex(unsigned long n)
{
	int	len;

	len = 1;
	while (n > 15)
	{
		n /= 16;
		len++;
	}
	return (len);
}

char	*ft_utohex(unsigned long n, char lower)
{
	int		len;
	char	*res;

	len = len_uint_hex(n);
	res = (char *)ft_calloc(len + 1, sizeof(char));
	if (!res)
		return (NULL);
	while (len)
	{
		if (lower)
			res[len - 1] = "0123456789abcdef"[n % 16];
		else
			res[len - 1] = "0123456789ABCDEF"[n % 16];
		n /= 16;
		len--;
	}
	return (res);
}
