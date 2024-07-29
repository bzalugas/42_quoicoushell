/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bazaluga <bazaluga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 22:14:35 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/29 10:13:25 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

//Handle multiple args + write exit from here
//+ Handle non numeric args
int	ft_exit(t_cmd *cmd, t_shell *sh)
{
	unsigned int	status;

	status = ft_atou_base(cmd->argv[1], "0123456789");
	exit_shell(sh, status % 256, true);
	return (0);
}
