/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:00:05 by bazaluga          #+#    #+#             */
/*   Updated: 2023/12/18 11:11:12 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	count_words(char const *s, char c)
{
	size_t	words;
	size_t	i;

	words = 0;
	i = 0;
	while (s[i])
	{
		if ((i == 0 && s[i] != c) || (i > 0 && s[i] != c && s[i - 1] == c))
			words++;
		i++;
	}
	return (words);
}

static size_t	len_word(char **s, char c)
{
	size_t	len;

	while (**s && **s == c)
		(*s)++;
	len = 0;
	while ((*s)[len] && (*s)[len] != c)
		len++;
	return (len);
}

static void	*big_free(char **arr)
{
	size_t	i;

	i = 0;
	while (arr && arr[i])
		free(arr[i++]);
	if (arr)
		free(arr);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	size_t	words;
	size_t	len;
	size_t	i;
	char	*tmp;
	char	**arr;

	if (!s)
		return (NULL);
	words = count_words(s, c);
	arr = (char **)ft_calloc(words + 1, sizeof(char *));
	if (!arr)
		return (NULL);
	tmp = (char *)s;
	i = 0;
	while (i < words)
	{
		len = len_word(&tmp, c);
		arr[i] = (char *)ft_calloc(len + 1, sizeof(char));
		if (!arr[i])
			return (big_free(arr));
		ft_strlcpy(arr[i], tmp, len + 1);
		tmp += len;
		i++;
	}
	return (arr);
}
