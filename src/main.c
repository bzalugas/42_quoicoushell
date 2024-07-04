/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:45:26 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/04 19:06:50 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../include/libft.h"

int	main(void)
{
	char	*cmd;

	while (1)
	{
		cmd = readline("QUOICOUSHELL >>> ");
		/* cmd = get_next_line(STDIN_FILENO); */
		if (!ft_strncmp(cmd, "END", 3))
		{
			free(cmd);
			exit(0);
		}
		free(cmd);
	}
	return (0);
}
