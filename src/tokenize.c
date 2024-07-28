/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 06:56:32 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/28 22:30:26 by jsommet          ###   ########.fr       */
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

char	*retrieve_var_name(char *p)
{
	int		i;
	char	*name;

	if (!p || p[0] != '$')
		return (NULL);
	i = 1;
	while (valid_name_char(p[i]))
		i++;
	if (ft_isquot(p[i]))
		return (ft_strdup(""));
	if (i == 1)
		return (NULL);
	name = (char *) calloc(i + 1, sizeof(char));
	i = 0;
	while (valid_name_char(p[++i]))
		name[i - 1] = p[i];
	return (name);
}

int	retrieve_var_value(t_shell *sh, char *p, char **value, int *name_size)
{
	char	*name;

	name = retrieve_var_name(p);
	if (!name)
		return (0);
	if (!value || !name_size)
		return (-1);
	*value = get_variable_value(sh, name);
	*name_size = ft_strlen(name);
	free(name);
	return (1);
}

int	var_word_count(t_shell *sh, char *p)
{
	int		i;
	int		words;
	char	*name;
	char	*content;

	name = retrieve_var_name(p);
	words = 0;
	content = get_variable_value(sh, name);
	if (!content)
		return (0);
	if (ft_isspace(content[0]))
		words++;
	i = 0;
	while (content[i])
		i++;
	if (ft_isspace(content[i - 1]))
		words++;
	words += count_wordsf(content, &ft_isspace);
	return (words);
}

int	count_new_words_in_word(t_shell *sh, char *word)
{
	int		c;
	char	q;
	int		i;

	i = 0;
	c = 0;
	while (word[i])
	{
		if (word[i] == q)
			q = 0;
		else if (word[i] == '"')
			q = '"';
		else if (!q && word[i] == '\'')
			i += next_quote(&word[i]);
		else if (word[i] == '$' && valid_name_char(word[i + 1]))
			c += var_word_count(sh, &word[i]);
		i++;
	}
	return (0);
}

typedef struct s_exvar //TODO: MOVE TO HEADER
{
	char	*value;
	int		start;
	int		len;
}	t_exvar;
/*

char	*prep_redir_word(t_shell *sh, char *word)
{
	char	*expanded;

	if (!word)
		return (NULL);
	expanded = expand_whole(sh, word);
	remove_quotes(expanded);
	return (expanded);
}

char	**expand_word(t_shell *sh, char *word)
{
	char	*name;


}
*/
// int	categorize_token(t_cmd *cmd, char **token, int *ctrs, t_shell *sh)
// {
// 	int	i;

// 	i = 0;
// 	if (!ft_strcmp(*token, "<<"))
// 	{
// 		cmd->heredocs[ctrs[1]++] = ft_strjoin(token[++i], "\n");
// 		cmd->heredoc = 1;
// 	}
// 	else if (!ft_strcmp(*token, "<") || !ft_strcmp(*token, ">")
// 		|| !ft_strcmp(*token, ">>"))
// 	{
// 		if (!ft_strcmp(*token, "<"))
// 			cmd->heredoc = 0;
// 		cmd->redirs[ctrs[2]].type = RTIN * !ft_strcmp(*token, "<")
// 			+ RTOUT_T * !ft_strcmp(*token, ">")
// 			+ RTOUT_A * !ft_strcmp(*token, ">>");
// 		cmd->redirs[ctrs[2]++].file = prep_redir_word(sh, token[++i]);
// 	}
// 	else
// 		//ft_memcpy(&cmd->argv[ctrs[0]++], words, byte size of words)
// 		cmd->argv[ctrs[0]++] = ft_strdup(*token);
// 	return (i);
// }

char	*prep(char *word, bool hd)
{
	char	*nw;

	if (hd)
		nw = ft_strjoin(word, "\n");
	else
		nw = ft_strdup(word);
	remove_quotes(nw);
	return (nw);
}

typedef struct s_quot
{
	int	start;
	int	size;
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

int	get_new_size(t_shell *sh, char *word, t_quot *qi)
{
	int		i;
	int		name_size;
	int		new_size;
	char	*tmp_val;

	(void) qi;
	i = 0;
	new_size = ft_strlen(word);
	while (word[i])
	{
		if (qi->size && i == qi->start)
		{
			i += qi->size;
			qi++;
			continue ;
		}
		if (retrieve_var_value(sh, &word[i], &tmp_val, &name_size) > 0)
		{
			new_size -= name_size + 1;
			new_size += ft_strlen(tmp_val);
			i += name_size + 1;
		}
		else
			i++;
	}
	return (new_size);
}

char	*simple_expand(t_shell *sh, char *word, t_quot *qi)
{
	int		i;
	int		j;
	char	*new_word;
	int		name_size;
	char	*tmp_val;

	i = 0;
	(void) qi;
	j = 0;
	new_word = (char *) calloc(get_new_size(sh, word, qi) + 1, sizeof(char));
	while (word[i])
	{
		if (qi->size && i == qi->start)
		{
			i += qi->size;
			qi++;
			continue ;
		}
		if (retrieve_var_value(sh, &word[i], &tmp_val, &name_size) > 0)
		{
			dprintf(2, "%s\n", tmp_val);
			if (tmp_val)
				ft_strcpy(&new_word[j], tmp_val);
			i += name_size + 1;
			j += ft_strlen(tmp_val);
		}
		else
			new_word[j++] = word[i++];
	}
	return (new_word);
}

void	alloc_quotes_position(char *word, char quot, t_quot **qi)
{
	int	qc;
	int	q;
	int	i;

	qc = 0;
	i = -1;
	while (word[++i])
	{
		q = next_quote(&word[i]);
		if (q > 0 && (!quot || quot == word[i]))
		{
			qc++;
			i += q;
		}
	}
	*qi = (t_quot *) calloc(qc + 1, sizeof(t_quot));
}

t_quot	*get_quotes_position(char *word, char quot)
{
	int		offset;
	int		qc;
	int		i;
	int		q;
	t_quot	*qi;


	alloc_quotes_position(word, quot, &qi);
	offset = 0;
	qc = 0;
	i = -1;
	while (word[++i])
	{
		q = next_quote(&word[i]);
		if (q > 0 && (!quot || quot == word[i]))
		{
			qi[qc++] = (t_quot){i - offset, q};
			offset += 2;
			i += q;
		}
	}
	return (qi);
}

char	*remove_quotes_and_expand(t_shell *sh, char *word)
{
	char	*expanded;
	t_quot	*qi;

	qi = get_quotes_position(word, '\'');
	remove_quotes(word);
	expanded = simple_expand(sh, word, qi);
	return (expanded);
}

void	set_heredoc(t_shell *sh, t_cbv *cbv)
{
	(void) sh;
	cbv->cmd->heredocs[cbv->hd_i++] = ft_strjoin(cbv->tks[++cbv->tk_i], "\n");
	cbv->cmd->heredoc = 1;
}

void	set_redir(t_shell *sh, t_cbv *cbv)
{
	if (!ft_strcmp(cbv->tks[cbv->tk_i], "<"))
		cbv->cmd->heredoc = 0;
	if (!ft_strcmp(cbv->tks[cbv->tk_i], "<"))
		cbv->cmd->redirs[cbv->rd_i].type = RTIN;
	else if (!ft_strcmp(cbv->tks[cbv->tk_i], ">"))
		cbv->cmd->redirs[cbv->rd_i].type = RTOUT_T;
	else if (!ft_strcmp(cbv->tks[cbv->tk_i], ">>"))
		cbv->cmd->redirs[cbv->rd_i].type = RTOUT_A;
	cbv->cmd->redirs[cbv->rd_i].file = remove_quotes_and_expand(sh, cbv->tks[++cbv->tk_i]);
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
	cbv->cmd->argv[cbv->arg_i++] = remove_quotes_and_expand(sh, cbv->tks[cbv->tk_i]);
}

void	set_cmd_word(t_shell *sh, t_cbv *cbv)
{
	//expand aand split

	//ft_memcpy(&cmd->argv[ctrs[0]++], split_expanded_word, wcount * sizeof(char *)) //free split_expanded_words
	cbv->cmd->argv[cbv->arg_i++] = remove_quotes_and_expand(sh, cbv->tks[cbv->tk_i]);
}

t_cmd	*init_cmd(t_shell *sh, char **tokens, int n)
{
	int		c[3];
	t_cmd	*cmd;

	(void) sh;
	ft_bzero(c, sizeof(c));
	cmd = (t_cmd *) calloc(1, sizeof(t_cmd));
	while (tokens[c[0]])
	{
		if (!ft_strcmp(tokens[c[0]], "<<"))
			c[1]++;
		else if (!ft_strcmp(tokens[c[0]], "<")
			|| !ft_strcmp(tokens[c[0]], ">") || !ft_strcmp(tokens[c[0]], ">>"))
			c[2]++;
		else
			cmd->argc += count_new_words_in_word(sh, tokens[c[0]]);
		if (!ft_strcmp(tokens[c[0]], "<<") || !ft_strcmp(tokens[c[0]], "<")
			|| !ft_strcmp(tokens[c[0]], ">") || !ft_strcmp(tokens[c[0]], ">>"))
			c[0]++;
		if (tokens[c[0]])
			c[0]++;
	}
	cmd->argv = (char **) calloc(cmd->argc + 1, sizeof(char *));
	cmd->heredocs = (char **) calloc(c[1] + 1, sizeof(char *));
	cmd->redirs = (t_redir *) calloc(c[2] + 1, sizeof(t_redir));
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
