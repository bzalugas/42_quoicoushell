/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strreplace.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:09:17 by bazaluga          #+#    #+#             */
/*   Updated: 2024/08/01 20:24:30 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**alloc_addr_arr(unsigned int occ)
{
	size_t	count;
	char	**arr;

	count = 0;
	while (occ)
	{
		count++;
		occ >>= 1;
	}
	arr = (char **) ft_calloc(count + 3, sizeof(void *));
	return (arr);
}

size_t	get_len_res(char *str, char **arr, unsigned int occ)
{
	size_t	to_replace_len;
	size_t	new_str_len;
	char	*addr;
	size_t	i;
	size_t	len;

	len = ft_strlen(str);
	to_replace_len = ft_strlen(arr[0]);
	new_str_len = ft_strlen(arr[1]);
	i = 2;
	while (occ)
	{
		addr = ft_strnstr(str, arr[0], ft_strlen(str));
		if (!addr)
			return (len);
		if (((occ & 0x80000000) == 0x80000000) && addr)
		{
			len = len - to_replace_len + new_str_len;
			arr[i++] = addr;
		}
		str = addr + to_replace_len;
		occ <<= 1;
	}
	return (len);
}

void	replace_strs(char *str, char **addrs, char *res)
{
	size_t	i;
	size_t	to_replace_len;
	size_t	new_str_len;

	to_replace_len = ft_strlen(addrs[0]);
	new_str_len = ft_strlen(addrs[1]);
	i = 2;
	while (str && *str && addrs[i])
	{
		if (str < addrs[i])
			ft_strlcat(res, str, ft_strlen(res) + (addrs[i] - str) + 1);
		ft_strlcat(res, addrs[1], (ft_strlen(res) + new_str_len) + 1);
		str = addrs[i] + to_replace_len;
		i++;
	}
	if (*str)
		ft_strlcat(res, str, ft_strlen(res) + ft_strlen(str) + 1);
}

/**
 * Replace a substring by another in a string, selecting wich occurences to
 * replace.
 *@param str: the big string
 *@param to_replace: the substring to replace
 *@param new_str: the string to put in place of to_replace
 *@param occ: in binary, the occurences to replace(0x80000000 for the first one)
 *@return: the new allocated string with the replaced substrings*/
char	*ft_strreplace(char *str, char *to_replace, char *new_str,
			unsigned int occ)
{
	char	*res;
	size_t	len_res;
	char	**arr;

	res = NULL;
	arr = alloc_addr_arr(occ);
	if (!arr)
		return (NULL);
	arr[0] = to_replace;
	arr[1] = new_str;
	len_res = get_len_res(str, arr, occ);
	res = (char *) ft_calloc(len_res + 1, sizeof(char));
	if (!res)
		return (NULL);
	replace_strs(str, arr, res);
	free(arr);
	return (res);
}
