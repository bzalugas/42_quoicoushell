/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 06:56:32 by jsommet           #+#    #+#             */
/*   Updated: 2024/09/30 19:50:26 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

// TODO: PROTECT MALLOCS (3)

	// cbv->cmd->heredocs[cbv->hd_i] = ft_strjoin(cbv->tks[++cbv->tk_i], "\n");
void	set_heredoc(t_shell *sh, t_cbv *cbv)
{
	(void) sh;
	cbv->cmd->heredocs[cbv->hd_i] = ft_strdup(cbv->tks[++cbv->tk_i]);
	if (!cbv->cmd->heredocs[cbv->hd_i])
		exit_shell(sh, EXIT_FAILURE, false);
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
