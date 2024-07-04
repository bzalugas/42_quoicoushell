/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_char_str_handlers.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 13:23:23 by bazaluga          #+#    #+#             */
/*   Updated: 2024/06/22 13:27:21 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

bool	handle_char(t_buffer *buf, t_node *node, int c)
{
	int		len;
	char	*res;
	bool	minus;
	int		width;

	width = ((t_flags *)node->content)->width;
	minus = ((t_flags *)node->content)->minus;
	len = width + (width < 1);
	res = ft_calloc(len + 1, sizeof(char));
	if (!res)
		return (false);
	ft_memset(res + minus, ' ', len - 1);
	res[(len - 1 + minus) % len] = (unsigned char)c;
	free(node->content);
	node->content = res;
	node->type = CONV;
	buf->tot_len += len;
	node->len = len;
	return (true);
}

static bool	handle_flags_str(t_buffer *buf, t_node *node, const char *str)
{
	int			len_add;
	int			len_str;
	t_flags		*f;
	char		*add;

	len_add = 0;
	f = (t_flags *)node->content;
	len_str = ft_strlen(str);
	if (f->dot && f->pad > len_str)
		f->dot = 0;
	if (f->dot && f->width > len_str - (len_str - f->pad))
		len_add = f->width - (len_str - (len_str - f->pad));
	else if (!f->dot)
		len_add = f->width - len_str;
	if (len_add < 1)
		return (true);
	add = (char *)ft_calloc(len_add + 1, sizeof(char));
	if (!add)
		return (false);
	ft_memset(add, ' ', len_add);
	if (f->minus && f->width)
		return (buff_add_after(buf, node, node_new(CONV, len_add, add)));
	return (buff_add_before(buf, node, node_new(CONV, len_add, add)));
}

bool	handle_str(t_buffer *buf, t_node *node, const char *str)
{
	t_flags	*f;

	if (!str && !handle_flags_str(buf, node, "(null)"))
		return (false);
	else if (str && !handle_flags_str(buf, node, str))
		return (false);
	f = (t_flags *)node->content;
	if (str && f->dot)
		node->content = ft_strndup(str, f->pad);
	else if (str)
		node->content = ft_strdup(str);
	else if (!str && f->pad < 6 && f->pad > 0)
		node->content = ft_strdup("");
	else
		node->content = ft_strdup("(null)");
	node->type = CONV;
	node->len = ft_strlen((char *)node->content);
	buf->tot_len += node->len;
	free(f);
	return (true);
}

bool	handle_percent(t_buffer *buf, t_node *node)
{
	free(node->content);
	node->content = ft_strdup("%");
	if (!node->content)
		return (false);
	node->len = 1;
	buf->tot_len += 1;
	return (true);
}
