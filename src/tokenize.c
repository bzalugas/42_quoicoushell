/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 06:56:32 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/30 02:01:26 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

/*
	- unmatched quotes
	- not a word before pipes
	- no file for given to redirection
	- no command given before pipe
	- newline after redirection

	melenshell: syntax error near unexpected token 'token's
*/

// TODO: PROTECT MALLOCS (3)

int	valid_name_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

typedef struct s_exvar //TODO: MOVE TO HEADER
{
	char	*value;
	int		start;
	int		len;
}	t_exvar;

typedef struct s_quot
{
	char	q;
	void	*start;
	size_t	size;
}	t_quot;

typedef struct s_cbv
{
	t_cmd	*cmd;
	char	**tks;
	int		arg_i;
	int		tk_i;
	int		hd_i;
	int		rd_i;
}	t_cbv;

typedef struct s_expand_data
{
	t_quot	*qi;
	t_quot	*tqi;
	char	*tmp_val;
	int		name_size;
}	t_expand_data;

char	*retrieve_var_name(char *p, t_expand_data *xdat)
{
	int		i;
	char	*name;

	if (!p || p[0] != '$' || !xdat)
		return (NULL);
	i = 1;
	while (valid_name_char(p[i]) && &p[i] != xdat->tqi->start)
		i++;
	if (i == 1 && &p[i + 1] != xdat->tqi->start)
		return (NULL);
	name = (char *) ft_calloc(i, sizeof(char));
	i = 0;
	while (valid_name_char(p[++i]) && &p[i] != xdat->tqi->start)
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

// int	var_word_count(t_shell *sh, char *p)
// {
// 	int		i;
// 	int		words;
// 	char	*name;
// 	char	*content;

// 	name = retrieve_var_name(p);
// 	words = 0;
// 	content = get_variable_value(sh, name);
// 	if (!content)
// 		return (0);
// 	if (ft_isspace(content[0]))
// 		words++;
// 	i = 0;
// 	while (content[i])
// 		i++;
// 	if (ft_isspace(content[i - 1]))
// 		words++;
// 	words += count_wordsf(content, &ft_isspace);
// 	return (words);
// }

// int	count_new_words_in_word(t_shell *sh, char *word)
// {
// 	int		c;
// 	char	q;
// 	int		i;

// 	i = 0;
// 	c = 0;
// 	while (word[i])
// 	{
// 		if (word[i] == q)
// 			q = 0;
// 		else if (word[i] == '"')
// 			q = '"';
// 		else if (!q && word[i] == '\'')
// 			i += next_quote(&word[i]);
// 		else if (word[i] == '$' && valid_name_char(word[i + 1]))
// 			c += var_word_count(sh, &word[i]);
// 		i++;
// 	}
// 	return (0);
// }

int	get_new_size(t_shell *sh, char *word, t_expand_data *xdat)
{
	int		i;
	int		new_size;

	i = 0;
	xdat->tqi = xdat->qi;
	new_size = ft_strlen(word);
	while (word[i])
	{
		if (xdat->tqi->size && &word[i] == xdat->tqi->start)
		{
			if (xdat->tqi->q == '\'')
				i += xdat->tqi->size;
			xdat->tqi++;
		}
		else if (retrieve_var_value(sh, &word[i], xdat) > 0)
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

char	*simple_expand(t_shell *sh, char *word, t_expand_data *xdat)
{
	int		i;
	int		j;
	char	*new_word;

	i = 0;
	j = 0;
	new_word = (char *) ft_calloc(get_new_size(sh, word, xdat) + 1, 1UL);
	xdat->tqi = xdat->qi;
	while (word[i])
	{
		if (xdat->tqi->size && &word[i] == xdat->tqi->start)
		{
			if (xdat->tqi->q == '\'')
				while (--xdat->tqi->size)
					new_word[j++] = word[i++];
			xdat->tqi++;
		}
		else if (retrieve_var_value(sh, &word[i], xdat) > 0)
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

void	alloc_quotes_position(char *word, t_quot **qi)
{
	int	qc;
	int	q;
	int	i;

	qc = 0;
	i = -1;
	while (word[++i])
	{
		q = next_quote(&word[i]);
		if (q > 0)
		{
			qc++;
			i += q;
		}
	}
	*qi = (t_quot *) ft_calloc(qc + 1, sizeof(t_quot));
}

t_quot	*get_quotes_position(char *word)
{
	int		offset;
	int		qc;
	int		i;
	int		q;
	t_quot	*qi;

	alloc_quotes_position(word, &qi);
	offset = 0;
	qc = 0;
	i = -1;
	while (word[++i])
	{
		q = next_quote(&word[i]);
		if (q > 0)
		{
			qi[qc++] = (t_quot){word[i], &word[i] - offset, q};
			offset += 2;
			i += q;
		}
	}
	return (qi);
}

char	*remove_quotes_and_expand(t_shell *sh, char *word)
{
	char			*expanded;
	t_expand_data	xdat;

	xdat.qi = get_quotes_position(word);
	remove_quotes(word);
	expanded = simple_expand(sh, word, &xdat);
	free(xdat.qi);
	return (expanded);
}

char	*create_quote_mask(char *word, t_quot *qi)
{
	char	*mask;
	int		size;
	int		i;

	i = 0;
	size = ft_strlen(word);
	mask = (char *) calloc(size + 1, sizeof(char));
	while (i < size)
	{
		if (qi->size && (int)(qi->start - (void *)word) == i)
		{
			i += qi->size;
			qi++;
		}
		else
		{
			mask[i] = 1;
			i++;
		}
	}
	return (mask);
}

char	**split_word(char *word, t_quot *qi)
{
	char	*mask;
	char	**split;

	mask = create_quote_mask(word, qi);
	split = ft_splitf_mask(word, ft_isspace, mask);
	free(mask);
	return (split);
}

void	set_heredoc(t_shell *sh, t_cbv *cbv)
{
	(void) sh;
	cbv->cmd->heredocs[cbv->hd_i] = ft_strjoin(cbv->tks[++cbv->tk_i], "\n");
	cbv->cmd->heredoc = true;
	cbv->hd_i++;
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

void	set_var_assign(t_shell *sh, t_cbv *cbv)
{
	cbv->cmd->argv[cbv->arg_i] = remove_quotes_and_expand(sh, cbv->tks[cbv->tk_i]);
	cbv->arg_i++;
}

void	ft_putstr_err(char *s)
{
	while (*s)
		write(2, s++, 1);
}

void	set_cmd_word(t_shell *sh, t_cbv *cbv)
{
	char	*simple_exp;
	char	**split;
	t_quot	*qi;

	qi = get_quotes_position(cbv->tks[cbv->tk_i]);
	simple_exp = remove_quotes_and_expand(sh, cbv->tks[cbv->tk_i]);
	split = split_word(simple_exp, qi);
	ft_memcpy(&cbv->cmd->argv[cbv->arg_i],
		split, ft_splitlen(split) * sizeof(char *));
	free(split);
	cbv->arg_i++;
}

int	split_expand_count(t_shell *sh, char *word)
{
	char	*simple_exp;
	char	*mask;
	t_quot	*qi;
	int		wc;

	qi = get_quotes_position(word);
	simple_exp = remove_quotes_and_expand(sh, word);
	mask = create_quote_mask(word, qi);
	wc = count_wordsf_mask(word, ft_isspace, mask);
	free(mask);
	return (wc);
}

t_cmd	*init_cmd(t_shell *sh, char **tokens, int n)
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
	cmd->n_cmd = n;
	return (cmd);
}

t_cmd	*get_command(t_shell *sh, char **tokens, int n)
{
	t_cbv		cbv;

	cbv = (t_cbv){0};
	cbv.cmd = init_cmd(sh, tokens, n);
	cbv.tks = tokens;
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
