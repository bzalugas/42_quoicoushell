/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 06:56:32 by jsommet           #+#    #+#             */
/*   Updated: 2024/10/29 22:31:22 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

void	set_heredoc(t_shell *sh, t_cbv *cbv)
{
	char	*word;

	word = ft_strdup(cbv->tks[++cbv->tk_i]);
	cbv->cmd->hd_expand[cbv->hd_i] = !remove_quotes(word);
	cbv->cmd->heredocs[cbv->hd_i] = word;
	if (!cbv->cmd->heredocs[cbv->hd_i])
		exit_shell(sh, EXIT_FAILURE, false);
	cbv->cmd->heredoc = true;
	cbv->hd_i++;
}

bool	can_be_var_assign(char *word, char **argv)
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
	return (argv[0] && !ft_strcmp(argv[0], "export"));
}

bool	is_var_assign(t_shell *sh, t_cbv *cbv, char *word)
{
	int	i;

	(void) sh;
	if (!can_be_var_assign(word, cbv->cmd->argv))
		return (false);
	i = -1;
	if (cbv->cmd->argv[0] && !ft_strcmp(cbv->cmd->argv[0], "export"))
		i++;
	while (cbv->cmd->argv[++i])
		if (!can_be_var_assign(cbv->cmd->argv[i], cbv->cmd->argv))
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
		= remove_quotes_and_expand(sh, cbv->tks[++cbv->tk_i], false);
	cbv->rd_i++;
}

void	set_var_assign(t_shell *sh, t_cbv *cbv)
{
	cbv->cmd->argv[cbv->arg_i]
		= remove_quotes_and_expand(sh, cbv->tks[cbv->tk_i], true);
	cbv->arg_i++;
}
