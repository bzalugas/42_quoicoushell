/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 22:03:54 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/29 14:04:56 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	ft_pwd(t_shell *sh)
{
	free(sh->cwd);
	sh->cwd = getcwd(NULL, 0);
	if (!sh->cwd)
		print_perror("pwd: error retrieving current directory", "getcwd");
	ft_putendl_fd(sh->cwd, STDOUT_FILENO);
	return (0);
}
