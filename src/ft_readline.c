/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 22:53:54 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/17 17:30:57 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

char	*ft_readline(char *prompt)
{
	char	*line;

	ft_putstr_fd(prompt, 1);
	line = get_next_line(0);
	return (line);
}
