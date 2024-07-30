/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strreplace.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 12:09:17 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/31 00:05:10 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//TODO: FIX THE FUNCTION
char	*ft_strreplace(char *str, char *to_replace, char *new_str)
{
	char	*res[2];
	char	*repl_addr;
	size_t	len_res;
	size_t	len_replace;
	/* size_t	i; */
	int		res_i;

	/* i = 0; */
	res_i = 0;
	res[0] = NULL;
	res[1] = NULL;
	len_replace = ft_strlen(to_replace);
	repl_addr = ft_strnstr(str, to_replace, ft_strlen(str));
	while (repl_addr)
	{
		len_res = ft_strlen(str) - len_replace + ft_strlen(new_str) + 1;
		res[res_i] = (char *)ft_calloc(len_res, sizeof(char));
		if (!res[res_i])
			return (NULL);
		ft_strlcpy(res[res_i], str, repl_addr - str);
		ft_dprintf(2, "res = %s\n", res[res_i]);
		ft_strlcat(res[res_i], new_str, ft_strlen(res[res_i])
			+ ft_strlen(new_str) + 1);
		ft_dprintf(2, "res = %s\n", res[res_i]);
		ft_strlcat(res[res_i], &str[ft_strlen(res[res_i])], len_res + 1);
		ft_dprintf(2, "res = %s\n", res[res_i]);
		res_i = !res_i;
		str = repl_addr + len_replace;
		repl_addr = ft_strnstr(str, to_replace, ft_strlen(str));
	}
	return (res[res_i]);
}
