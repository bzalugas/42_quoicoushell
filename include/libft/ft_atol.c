/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 04:30:11 by bazaluga          #+#    #+#             */
/*   Updated: 2024/02/23 17:31:24 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_atol(const char *nptr)
{
	long	n;
	int		sign;
	size_t	i;

	i = 0;
	while (nptr[i] && ft_isspace(nptr[i]))
		i++;
	sign = 1;
	if (nptr[i] && (nptr[i] == '-' || nptr[i] == '+'))
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	n = 0;
	while (nptr[i] && ft_isdigit(nptr[i]))
	{
		n = n * 10 + (nptr[i] - '0');
		i++;
	}
	return (n * sign);
}
