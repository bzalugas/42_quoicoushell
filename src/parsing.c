/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 16:03:11 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/27 18:04:31 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

void	check_for_open_quotes(char *line)
{
	int	i;
	int	q;

	i = 0;
	while (line[i])
	{
		q = next_quote(&line[i]);
		if (q > 0)
			i += q;
		else if (q == 0)
			;// syntax_error("open quote"); //abort line
		i++;
	}
}
