/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_paths.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 20:27:59 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/29 11:21:29 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

static void	*free_broken_split(char **arr, size_t broken_idx)
{
	size_t	i;

	if (!arr)
		return (NULL);
	i = 0;
	while (i < broken_idx)
		free(arr[i++]);
	i++;
	while (arr[i])
		free(arr[i++]);
	return (NULL);
}

char	**get_paths(char **env)
{
	size_t	i;
	char	**paths;

	if (!env)
		return (NULL);
	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (env[i])
		paths = ft_split(&env[i][5], ':');
	else
		return (NULL);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		paths[i] = ft_strjoin_free(paths[i], "/", 1, 0);
		if (!paths[i])
			return (free_broken_split(paths, i));
		i++;
	}
	return (paths);
}
