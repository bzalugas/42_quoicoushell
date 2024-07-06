/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 02:51:47 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/06 05:48:35 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

char	**split_env_entry(char *entry)
{
	char	**split;
	int		i;
	int		j;

	split = (char **) calloc(3, sizeof(char *));
	i = 0;
	while (entry[i] != '=')
		i++;
	split[0] = (char *) malloc(sizeof(char) * (i + 1));
	j = 1;
	while (entry[i + j])
		j++;
	split[1] = (char *) malloc(sizeof(char) * (j + 1));
	i = -1;
	while (entry[++i] != '=')
		split[0][i] = entry[i];
	split[0][i] = '\0';
	j = 0;
	while (entry[i + (++j)])
		split[1][j] = entry[i + j];
	split[1][j] = '\0';
	return (split);
}
