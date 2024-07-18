/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 06:56:32 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/18 15:01:51 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

/*
	y- unmatched quotes
	o- not a word before pipes
	- back to back operators, |< |> |<< |>> because these take a name after)
	- newline after redirection

	melenshell: syntax error near unexpected token 'token's
*/
// int	check_line_syntax(char *line)
// {
// 	int	i;
// 	int	q;

// 	i = 0;
// 	while (line[i])
// 	{
// 		q = next_quote(&line[i]);
// 		if (q == 0)
// 			ft_putendl_fd("syntax error: unmatched quotes", 2);
// 		if (q > 0)
// 			i += q;
// 		if ((line[i] == '<' && line[i + 1] == '>')
// 			|| (line[i] == '>' && line[i + 1] == '<'))
// 			ft_putendl_fd("syntax error: near unexpected token '<' or '>'", 2);
// 	}
// 	return (0);
// }

t_cmd	*init_cmd(char **tokens, int n)
{
	t_cmd	*cmd;
	int		hdc;
	int		c;
	int		i;

	i = 0;
	c = 0;
	hdc = 0;
	cmd = (t_cmd *) calloc(1, sizeof(t_cmd));
	while (tokens[i])
	{
		if (!ft_strcmp(tokens[i], "<<"))
		{
			i++;
			hdc++;
		}
		else if (!ft_strcmp(tokens[i], "<")
			|| !ft_strcmp(tokens[i], ">") || !ft_strcmp(tokens[i], ">>"))
		{
			i++;
			c++;
		}
		else
			cmd->argc++;
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

t_cmd	*get_command(char **tokens, int n)
{
	int			i;
	int			arg;
	int			red;
	int			hdc;
	t_cmd		*cmd;

	i = 0;
	hdc = 0;
	arg = 0;
	red = 0;
	cmd = init_cmd(tokens, n);
	while (tokens[i])
	{
		if (!ft_strcmp(tokens[i], "<<"))
		{
			cmd->heredocs[hdc++] = ft_strdup(tokens[++i]);
			cmd->heredoc = 1;
		}
		else if (!ft_strcmp(tokens[i], "<") || !ft_strcmp(tokens[i], ">")
			|| !ft_strcmp(tokens[i], ">>"))
		{
			if (!ft_strcmp(tokens[i], "<"))
				cmd->heredoc = 0;
			cmd->redirs[red].type = RTIN * !ft_strcmp(tokens[i], "<")
				+ RTOUT_T * !ft_strcmp(tokens[i], ">")
				+ RTOUT_A * !ft_strcmp(tokens[i], ">>");
			cmd->redirs[red++].file = ft_strdup(tokens[++i]);
		}
		else
			cmd->argv[arg++] = ft_strdup(tokens[i]);
		if (tokens[i])
			i++;
	}
	return (cmd);
}
// TODO: PROTECT MALLOCS (strdups)

int	count_new_words_in_word(char *word)
{
	(void) word;
	return (0);
}

char	**expand_vars(char **words)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (words[i])
		count += count_new_words_in_word(words[i++]);

	return (NULL);
}

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
