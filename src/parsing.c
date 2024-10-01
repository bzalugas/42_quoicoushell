/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 16:03:11 by jsommet           #+#    #+#             */
/*   Updated: 2024/09/30 20:08:51 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

t_cmd	*init_cmd(t_shell *sh, char **tokens)
{
	int		c[3];
	t_cmd	*cmd;

	(void) sh;
	cmd = (t_cmd *) ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	ft_bzero(c, sizeof(c));
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
	if (!cbv.cmd || !cbv.cmd->argv || !cbv.cmd->heredocs || !cbv.cmd->redirs)
		return (NULL);
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
