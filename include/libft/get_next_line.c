/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:33:32 by bazaluga          #+#    #+#             */
/*   Updated: 2024/03/07 13:16:58 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*remain_clear(t_remain *buf)
{
	t_remain	*tmp;

	while (buf)
	{
		tmp = buf->next;
		free(buf->read);
		free(buf);
		buf = tmp;
	}
	return (NULL);
}

static t_remain	*remain_get_new(t_remain *last, char alloc_read)
{
	t_remain	*new;
	char		*read;

	new = (t_remain *)ft_calloc_gnl(NULL, 1, sizeof(t_remain));
	read = NULL;
	if (alloc_read)
		read = (char *)ft_calloc_gnl(NULL, BUFFER_SIZE + 1UL, sizeof(char));
	if (!new || (alloc_read && !read))
		return (free(new), free(read), NULL);
	new->read = read;
	new->nl = NULL;
	new->next = NULL;
	if (!last)
		return (new);
	last->next = new;
	new->n = last->n + 1;
	return (new);
}

static char	*get_remaining_line(char *remain, t_remain *buf)
{
	char	*line;
	size_t	len_remain;
	size_t	len_nl;

	if (!*remain || !buf)
		return (remain_clear(buf));
	len_remain = (buf->nl - remain) + 1UL;
	if (!ft_calloc_gnl(&line, len_remain + 1UL, sizeof(char)))
		return (remain_clear(buf));
	ft_memmove(line, remain, len_remain);
	len_nl = ft_strlen(buf->nl + 1);
	ft_memmove(remain, buf->nl + 1UL, len_nl);
	ft_memset(remain + len_nl, '\0', BUFFER_SIZE - len_nl);
	remain_clear(buf);
	return (line);
}

static char	*create_save_line(t_remain *buf, t_remain *last, char *mem)
{
	size_t		len_l;
	size_t		len_mem;
	char		*l;
	size_t		i;
	t_remain	*tmp;

	len_mem = ft_strlen(mem);
	len_l = len_mem + (last->n * BUFFER_SIZE) + (last->nl - last->read);
	if (!ft_calloc_gnl(&l, len_l + (*last->nl == '\n') + 1UL, sizeof(char)))
		return (remain_clear(buf));
	ft_memmove(l, mem, len_mem);
	ft_memset(mem, '\0', BUFFER_SIZE);
	while (buf)
	{
		i = len_mem + (buf->n * BUFFER_SIZE);
		ft_memmove(l + i, buf->read, buf->nl - buf->read + (*buf->nl == '\n'));
		if (*buf->nl == '\n')
			ft_memmove(mem, buf->nl + 1UL, ft_strlen(buf->nl + 1UL));
		tmp = buf->next;
		free(buf->read);
		free(buf);
		buf = tmp;
	}
	return (l);
}

char	*get_next_line(int fd)
{
	static char	remainder[FD_MAX][BUFFER_SIZE + 1UL];
	t_remain	*buf;
	t_remain	*last;
	ssize_t		ret;

	if (fd < 0 || read(fd, NULL, 0) < 0)
		return (NULL);
	buf = remain_get_new(NULL, 0);
	if (ft_find_nl(remainder[fd], &buf->nl) > 0)
		return (get_remaining_line(remainder[fd], buf));
	if (!ft_calloc_gnl(&buf->read, BUFFER_SIZE + 1UL, sizeof(char)))
		return (NULL);
	ret = read(fd, buf->read, BUFFER_SIZE);
	last = buf;
	while (!ft_find_nl(last->read, &last->nl) && ret == BUFFER_SIZE)
	{
		ret = -1;
		last->next = remain_get_new(last, 1);
		last = last->next;
		if (last)
			ret = read(fd, last->read, BUFFER_SIZE);
	}
	if (ret == -1 || (ret == 0 && !buf->read[0] && !remainder[fd][0]))
		return (remain_clear(buf));
	return (create_save_line(buf, last, remainder[fd]));
}
