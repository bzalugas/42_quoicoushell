/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_buffer2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 13:23:12 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/30 18:06:25 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

void	*buff_clear(t_buffer **buf)
{
	t_node	*tmp;
	t_node	*tmp2;

	if (!buf || !*buf)
		return (NULL);
	tmp = (*buf)->first;
	while (tmp)
	{
		tmp2 = tmp->next;
		if (tmp->type != LIT)
			free(tmp->content);
		free(tmp);
		tmp = tmp2;
	}
	free(*buf);
	return (NULL);
}

static size_t	buffcpy(char *dst, t_buffer *buf)
{
	int		tot;
	t_node	*node;

	tot = 0;
	node = buf->first;
	while (node)
	{
		if (node->len > 0)
		{
			if (!ft_memcpy(dst + tot, node->content, node->len))
				return (tot);
			tot += node->len;
		}
		node = node->next;
	}
	return (tot);
}

size_t	buff_print(t_buffer *buf, int fd)
{
	char	*str;
	ssize_t	ret;

	if (!buf)
		return (0);
	str = (char *)ft_calloc(buf->tot_len + 1L, sizeof(char));
	if (!str)
		return (0);
	if (buffcpy(str, buf) != buf->tot_len)
	{
		free(str);
		return (0);
	}
	ret = write(fd, str, buf->tot_len);
	while (ret != (ssize_t)buf->tot_len)
	{
		if (ret == -1)
		{
			free(str);
			return (0);
		}
		ret += write(fd, str, buf->tot_len - ret);
	}
	free(str);
	return ((int)ret);
}
