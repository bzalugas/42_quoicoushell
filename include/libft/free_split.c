/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 19:28:57 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/01 11:58:19 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../quoicoushell.h"

int	free_split(char **arr)
{
	size_t	i;

	if (!arr)
		return (0);
	i = 0;
	while (arr[i])
	{
		free(arr[i++]);
	}
	free(arr);
	return (0);
}
