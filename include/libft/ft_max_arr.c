/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_max_arr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 19:08:20 by bazaluga          #+#    #+#             */
/*   Updated: 2024/05/26 19:08:27 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>

int	*ft_max_arr(int *arr, size_t len)
{
	size_t	i;
	int		*max;

	if (!arr)
		return ((void *)0);
	max = &arr[0];
	i = 1;
	while (i < len)
	{
		if (arr[i] > *max)
			max = &arr[i];
		i++;
	}
	return (max);
}
