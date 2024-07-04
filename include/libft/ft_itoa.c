/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 15:10:00 by bazaluga          #+#    #+#             */
/*   Updated: 2023/12/18 11:37:25 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	int_size(long n)
{
	size_t	size;

	size = 1;
	while (n > 9)
	{
		size++;
		n /= 10;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	long	n2;
	char	*nb;
	int		neg;
	size_t	size;

	n2 = n;
	neg = 0;
	if (n2 < 0 && ++neg)
		n2 *= -1;
	size = int_size(n2);
	nb = (char *)ft_calloc(size + neg + 1, sizeof(char));
	if (!nb)
		return (NULL);
	size += neg;
	while (size > 0)
	{
		nb[size - 1] = n2 % 10 + '0';
		n2 /= 10;
		size--;
	}
	if (neg)
		nb[0] = '-';
	return (nb);
}
