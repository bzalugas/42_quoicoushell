/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 12:33:39 by bazaluga          #+#    #+#             */
/*   Updated: 2024/02/23 20:29:54 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4095
# endif
# define FD_MAX 1024
# include <limits.h>

typedef struct s_remain
{
	char			*read;
	char			*nl;
	unsigned int	n;
	struct s_remain	*next;
}				t_remain;

/********************************* UTILS **************************************/

void	*ft_memset(void	*s, int c, size_t n);
int		ft_find_nl(const char *s, char **nl);
void	*ft_calloc_gnl(char **dst, size_t nmemb, size_t size);
void	*ft_memmove(void *dst, const void *src, size_t n);
size_t	ft_strlen(const char *s);

/********************************** GNL ***************************************/

char	*get_next_line(int fd);

#endif
