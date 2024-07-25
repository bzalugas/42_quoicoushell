/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 02:51:47 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/23 22:45:46 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

char	**split_env_entry(char *entry)
{
	char	**split;
	int		i;

	split = (char **) calloc(3, sizeof(char *));
	i = 0;
	while (entry[i] != '=')
		i++;
	split[0] = ft_substr(entry, 0, i);
	i++;
	split[1] = ft_strdup(entry + i);
	return (split);
}
