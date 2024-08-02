/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 06:56:32 by jsommet           #+#    #+#             */
/*   Updated: 2024/08/02 04:49:33 by bazaluga         ###   ########.fr       */
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

void	syntax_error(char *token)
{
	//syntax error near token "token".
	(void) token;
}

t_cmd	*init_cmd(t_shell *sh, char **tokens, int n)
{
	t_cmd	*cmd;
	int		hdc;
	int		c;
	int		i;

	(void) sh;
	i = 0;
	c = 0;
	hdc = 0;
	cmd = (t_cmd *) calloc(1, sizeof(t_cmd));
	while (tokens[i])
	{
		if (!ft_strcmp(tokens[i], "<<"))
			hdc++;
		else if (!ft_strcmp(tokens[i], "<")
			|| !ft_strcmp(tokens[i], ">") || !ft_strcmp(tokens[i], ">>"))
			c++;
		else
			cmd->argc++;
			// cmd->argc += count_new_words_in_word(sh, tokens[i]);
		if (!ft_strcmp(tokens[i], "<<") || !ft_strcmp(tokens[i], "<")
			|| !ft_strcmp(tokens[i], ">") || !ft_strcmp(tokens[i], ">>"))
			i++;
		if (tokens[i])
			i++;
	}
	cmd->argv = (char **) calloc(cmd->argc + 1, sizeof(char *));
	cmd->heredocs = (char **) calloc(hdc + 1, sizeof(char *));
	cmd->redirs = (t_redir *) calloc(c + 1, sizeof(t_redir));
	cmd->n_cmd = n;
	return (cmd);
}
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
	if (i == 1)
		return (NULL);
	name = (char *) calloc(i + 1, sizeof(char));
	i = 0;
	while (valid_name_char(p[++i]))
		name[i - 1] = p[i];
	return (name);
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
	if (ft_isspace(content[0]))
		words++;
	i = 0;
	while (content[i])
		i++;
	if (ft_isspace(content[i - 1]))
		words++;
	// words += count_wordsf(content, ft_isspace);
	return (words);
}

/* int	count_new_words_in_word(t_shell *sh, char *word) */
/* { */
/* 	int		c; */
/* 	char	q; */
/* 	int		i; */

/* 	i = 0; */
/* 	c = 0; */
/* 	while (word[i]) */
/* 	{ */
/* 		if (word[i] == q) */
/* 			q = 0; */
/* 		else if (word[i] == '"') */
/* 			q = '"'; */
/* 		else if (!q && word[i] == '\'') */
/* 			i += next_quote(&word[i]); */
/* 		else if (word[i] == '$' && valid_name_char(word[i + 1])) */
/* 			c += var_word_count(sh, &word[i]); */
/* 		i++; */
/* 	} */
/* 	return (0); */
/* } */

typedef struct s_exvar //TODO: MOVE TO HEADER
{
	char	*value;
	int		start;
	int		len;
}	t_exvar;
/*
char	*expand_whole(t_shell *sh, char *word)
{
	//course of action to be determined
}

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

int	categorize_token(t_cmd *cmd, char **token, int *ctrs, t_shell *sh)
{
	int	i;

	i = 0;
	(void) sh;
	if (!ft_strcmp(*token, "<<"))
	{
		cmd->heredocs[ctrs[1]++] = prep(token[++i], true);
		cmd->heredoc = 1;
	}
	else if (!ft_strcmp(*token, "<") || !ft_strcmp(*token, ">")
		|| !ft_strcmp(*token, ">>"))
	{
		if (!ft_strcmp(*token, "<"))
			cmd->heredoc = 0;
		cmd->redirs[ctrs[2]].type = RTIN * !ft_strcmp(*token, "<")
			+ RTOUT_T * !ft_strcmp(*token, ">")
			+ RTOUT_A * !ft_strcmp(*token, ">>");
		cmd->redirs[ctrs[2]++].file = prep(token[++i], false);
	}
	else
		//ft_memcpy(&cmd->argv[ctrs[0]++], words, byte size of words)
		cmd->argv[ctrs[0]++] = prep(token[i], false);
	return (i);
}

t_cmd	*get_command(t_shell *sh, char **tokens, int n)
{
	int			i;
	int			counters[3];
	t_cmd		*cmd;

	i = 0;
	ft_bzero(counters, sizeof(int) * 3);
	cmd = init_cmd(sh, tokens, n);
	while (tokens[i])
	{
		i += categorize_token(cmd, &tokens[i], counters, sh);
		if (tokens[i])
			i++;
	}
	return (cmd);
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
