/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 22:03:54 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/29 17:13:25 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	ft_pwd(t_shell *sh)
{
	free(sh->cwd);
	sh->cwd = getcwd(NULL, 0);
	if (!sh->cwd)
		print_perror("pwd: error retrieving current directory", "getcwd");
	sh->exit_code = !ft_putendl_fd(sh->cwd, STDOUT_FILENO);
	if (sh->exit_code)
		return (perror("quoicoushell: pwd: write error"), sh->exit_code);
	return (sh->exit_code);
}
