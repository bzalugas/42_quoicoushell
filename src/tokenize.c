/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 06:56:32 by jsommet           #+#    #+#             */
/*   Updated: 2024/09/16 16:38:14 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

// TODO: PROTECT MALLOCS (3)

void	set_heredoc(t_shell *sh, t_cbv *cbv)
{
	(void) sh;
	cbv->cmd->heredocs[cbv->hd_i] = ft_strjoin(cbv->tks[++cbv->tk_i], "\n");
	cbv->cmd->heredoc = true;
	cbv->hd_i++;
}

bool	can_be_var_assign(char *word)
{
	int	i;

	i = 0;
	if (!word)
		return (false);
	while (word[i])
	{
		if (i > 0 && word[i] == '=')
			return (true);
		if (!valid_name_char(word[i]))
			return (false);
		i++;
	}
	return (false);
}

bool	is_var_assign(t_shell *sh, t_cbv *cbv, char *word)
{
	int	i;

	(void) sh;
	if (!can_be_var_assign(word))
		return (false);
	i = -1;
	while (cbv->cmd->argv[++i])
		if (!can_be_var_assign(cbv->cmd->argv[i]))
			return (false);
	return (true);
}

void	set_redir(t_shell *sh, t_cbv *cbv)
{
	if (!ft_strcmp(cbv->tks[cbv->tk_i], "<"))
		cbv->cmd->heredoc = false;
	if (!ft_strcmp(cbv->tks[cbv->tk_i], "<"))
		cbv->cmd->redirs[cbv->rd_i].type = RTIN;
	else if (!ft_strcmp(cbv->tks[cbv->tk_i], ">"))
		cbv->cmd->redirs[cbv->rd_i].type = RTOUT_T;
	else if (!ft_strcmp(cbv->tks[cbv->tk_i], ">>"))
		cbv->cmd->redirs[cbv->rd_i].type = RTOUT_A;
	cbv->cmd->redirs[cbv->rd_i].file
		= remove_quotes_and_expand(sh, cbv->tks[++cbv->tk_i]);
	cbv->rd_i++;
}

void	set_var_assign(t_shell *sh, t_cbv *cbv)
{
	cbv->cmd->argv[cbv->arg_i]
		= remove_quotes_and_expand(sh, cbv->tks[cbv->tk_i]);
	cbv->arg_i++;
}

void	ft_putstr_err(char *s)
{
	while (*s)
		write(2, s++, 1);
}

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

/*
replace quotes ('\'' and '"') with (-1 and -2) and expand variables
replace every whitespace outside of "quotes" with -3
split with -3 as separator
*/

void	set_cmd_word(t_shell *sh, t_cbv *cbv)
{
	t_expand_data	xdat;
	char			*word;
	char			**split;

	word = cbv->tks[cbv->tk_i];
	xdat.new_size = get_new_size(sh, word, &xdat);
	replace_quotes(word);
	word = expand(sh, word, &xdat);
	replace_wsp(word);
	remove_weird_quotes(word);
	split = ft_split(word, C_WSP);
	free(word);
	split_cpy(&cbv->cmd->argv[cbv->arg_i],
		split, ft_splitlen(split));
	cbv->arg_i += ft_splitlen(split);
	free(split);
}

int	split_expand_count(t_shell *sh, char *word)
{
	int				wc;
	t_expand_data	xdat;
	char			*ogword;

	ogword = ft_strdup(word);
	xdat.new_size = get_new_size(sh, word, &xdat);
	replace_quotes(ogword);
	word = expand(sh, ogword, &xdat);
	free(ogword);
	replace_wsp(word);
	remove_weird_quotes(word);
	wc = count_words(word, C_WSP);
	free(word);
	return (wc);
}
