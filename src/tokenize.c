/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 06:56:32 by jsommet           #+#    #+#             */
/*   Updated: 2024/09/12 14:42:14 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"


// TODO: PROTECT MALLOCS (3)

int	valid_name_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	*retrieve_var_name(char *p, t_expand_data *xdat)
{
	int		i;
	char	*name;

	if (!p || p[0] != '$' || !xdat)
		return (NULL);
	i = 1;
	while (valid_name_char(p[i]))
		i++;
	if (i == 1 && p[i] == '"')
		return (NULL);
	name = (char *) ft_calloc(i, sizeof(char));
	i = 0;
	while (valid_name_char(p[++i]))
		name[i - 1] = p[i];
	return (name);
}

int	retrieve_var_value(t_shell *sh, char *p, t_expand_data *xdat)
{
	char	*name;

	name = retrieve_var_name(p, xdat);
	if (!name)
		return (0);
	if (!xdat)
		return (-1);
	xdat->tmp_val = get_variable_value(sh, name);
	xdat->name_size = ft_strlen(name);
	free(name);
	return (1);
}

int	get_new_size(t_shell *sh, char *word, t_expand_data *xdat)
{
	int		i;
	int		new_size;

	i = 0;
	new_size = ft_strlen(word);
	while (word[i])
	{
		if (word[i] == '\'')
			i += next_quote(&word[i]);
		if (retrieve_var_value(sh, &word[i], xdat) > 0)
		{
			new_size -= xdat->name_size + 1;
			new_size += ft_strlen(xdat->tmp_val);
			i += xdat->name_size + 1;
		}
		else
			i++;
	}
	return (new_size);
}

char	*expand(t_shell *sh, char *word, t_expand_data *xdat)
{
	int		i;
	int		j;
	int		quote_size;
	char	*new_word;

	i = 0;
	j = 0;
	new_word = (char *) ft_calloc(xdat->new_size + 1, 1UL);
	while (word[i])
	{
		if (word[i] == C_SQ)
		{
			quote_size = next_quote(&word[i]);
			while (--quote_size > 0)
				new_word[j++] = word[i++];
		}
		if (retrieve_var_value(sh, &word[i], xdat) > 0)
		{
			if (xdat->tmp_val)
				ft_strcpy(&new_word[j], xdat->tmp_val);
			i += xdat->name_size + 1;
			j += ft_strlen(xdat->tmp_val);
		}
		else
			new_word[j++] = word[i++];
	}
	return (new_word);
}

void	replace_quotes(char *word)
{
	int	i;

	i = 0;
	while (word[i])
	{
		if (word[i] == '\'')
			word[i] = C_SQ;
		if (word[i] == '"')
			word[i] = C_DQ;
		i++;
	}
}

void	replace_wsp(char *word)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (word[i])
	{
		if (!quote && (word[i] == C_SQ || word[i] == C_DQ))
			quote = word[i];
		else if (quote == word[i])
			quote = 0;
		if (!quote && ft_isspace(word[i]))
			word[i] = C_WSP;
		i++;
	}
}

void	remove_weird_quotes(char *word)
{
	int		offset;
	int		i;
	int		q;
	char	c;

	offset = 0;
	i = -1;
	c = 0;
	while (word[++i])
	{
		word[i - offset] = word[i];
		q = (word[i] == C_SQ || word[i] == C_DQ);
		if (c && c == word[i])
		{
			offset++;
			c = 0;
		}
		else if (!c && q > 0)
		{
			offset++;
			c = word[i];
		}
	}
	while (word[i - offset])
		word[i++ - offset] = 0;
}

char	*remove_quotes_and_expand(t_shell *sh, char *word)
{
	t_expand_data	xdat;

	xdat.new_size = get_new_size(sh, word, &xdat);
	replace_quotes(word);
	word = expand(sh, word, &xdat);
	remove_weird_quotes(word);
	return (word);
}

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
	split_cpy(&cbv->cmd->argv[cbv->arg_i],
		split, ft_splitlen(split));
	cbv->arg_i += ft_splitlen(split);
	free(split);
}

int	split_expand_count(t_shell *sh, char *word)
{
	int				wc;
	t_expand_data	xdat;

	word = ft_strdup(word);
	xdat.new_size = get_new_size(sh, word, &xdat);
	replace_quotes(word);
	word = expand(sh, word, &xdat);
	replace_wsp(word);
	remove_weird_quotes(word);
	wc = count_words(word, C_WSP);
	return (wc);
}

t_cmd	*init_cmd(t_shell *sh, char **tokens)
{
	int		c[3];
	t_cmd	*cmd;

	(void) sh;
	ft_bzero(c, sizeof(c));
	cmd = (t_cmd *) ft_calloc(1, sizeof(t_cmd));
	while (tokens[c[0]])
	{
		if (!ft_strcmp(tokens[c[0]], "<<"))
			c[1]++;
		else if (!ft_strcmp(tokens[c[0]], "<")
			|| !ft_strcmp(tokens[c[0]], ">") || !ft_strcmp(tokens[c[0]], ">>"))
			c[2]++;
		else
			cmd->argc += split_expand_count(sh, tokens[c[0]]);
		if (!ft_strcmp(tokens[c[0]], "<<") || !ft_strcmp(tokens[c[0]], "<")
			|| !ft_strcmp(tokens[c[0]], ">") || !ft_strcmp(tokens[c[0]], ">>"))
			c[0]++;
		if (tokens[c[0]])
			c[0]++;
	}
	cmd->argv = (char **) ft_calloc(cmd->argc + 1, sizeof(char *));
	cmd->heredocs = (char **) ft_calloc(c[1] + 1, sizeof(char *));
	cmd->redirs = (t_redir *) ft_calloc(c[2] + 1, sizeof(t_redir));
	return (cmd);
}

t_cmd	*get_command(t_shell *sh, char **tokens, int n)
{
	t_cbv		cbv;

	cbv = (t_cbv){0};
	cbv.cmd = init_cmd(sh, tokens);
	cbv.tks = tokens;
	cbv.cmd->n_cmd = n;
	while (tokens[cbv.tk_i])
	{
		if (!ft_strcmp(cbv.tks[cbv.tk_i], "<<"))
			set_heredoc(sh, &cbv);
		else if (!ft_strcmp(cbv.tks[cbv.tk_i], "<")
			|| !ft_strcmp(cbv.tks[cbv.tk_i], ">")
			|| !ft_strcmp(cbv.tks[cbv.tk_i], ">>"))
			set_redir(sh, &cbv);
		else if (is_var_assign(sh, &cbv, cbv.tks[cbv.tk_i]))
			set_var_assign(sh, &cbv);
		else
			set_cmd_word(sh, &cbv);
		if (tokens[cbv.tk_i])
			cbv.tk_i++;
	}
	return (cbv.cmd);
}
// TODO: PROTECT MALLOCS (strdups)

// char	**expand_vars(char **words)
// {
// 	int	count;
// 	int	i;

// 	i = 0;
// 	count = 0;
// 	while (words[i])
// 		count += count_new_words_in_word(words[i++]);

// 	return (NULL);
// }

// char	**expand(char *word)
// {
// 	char	**split_var;
// 	int		i;
// 	int		vc;

// 	while (word[i])
// 	{
// 		if (word[i] == '$' && valid_variable_character(word[i + 1]))
// 			vc ++;
// 		i++;
// 	}
// }
