/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 18:54:16 by jsommet           #+#    #+#             */
/*   Updated: 2024/10/29 20:19:58 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

void	split_cpy(char **dest, char **src, size_t src_len)
{
	int	i;

	i = 0;
	while (i < (int) src_len)
	{
		dest[i] = src[i];
		i++;
	}
}

void	set_cmd_word(t_shell *sh, t_cbv *cbv)
{
	t_expand_data	xdat;
	char			*word;
	char			**split;

	word = cbv->tks[cbv->tk_i];
	xdat.va = false;
	xdat.full_str = word;
	xdat.new_size = get_new_size(sh, word, &xdat);
	replace_quotes(word);
	expand(sh, word, &xdat);
	word = xdat.new_word;
	if (!word)
		exit_shell(sh, EXIT_FAILURE, false);
	replace_wsp(word);
	remove_weird_quotes(word);
	split = ft_split(word, C_WSP);
	free(word);
	split_cpy(&cbv->cmd->argv[cbv->arg_i],
		split, ft_splitlen(split));
	cbv->arg_i += ft_splitlen(split);
	free(split);
}
