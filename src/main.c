/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:45:26 by bazaluga          #+#    #+#             */
/*   Updated: 2024/10/08 14:21:40 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "quoicoushell.h"

int	g_sig = 0;

t_shell	*init_shell(char **envp)
{
	static t_shell sh = (t_shell){0};

	if (!sh.cwd)
	{
		set_signals_main(&sh);
		import_env(&sh, envp);
		sh.cwd = getcwd(NULL, 0);
		sh.prompt = build_prompt(&sh);
		sh.env_update = true;
		set_variable(&sh, ft_strdup("SHLVL"), ft_itoa(ft_atoi(
					get_variable_value(&sh, "SHLVL")) + 1), LST_ENV);
	}
	return (&sh);
}

char *get_redir_type(t_redir_type type)
{
	switch (type)
	{
		case RTIN: return "IN";
		case RTOUT_A: return "OUT_APPEND";
		case RTOUT_T: return "OUT_TRUNC";
	}
	return (NULL);
}

void	command_line(t_shell *sh, char *line)
{
	char		**tokens;
	t_tokens	t;
	t_cmd		*cmd;
	t_lstcmds	cmds;

	if (!line)
		return ;
	if (!check_syntax(line))
	{
		ft_putstr_fd("Syntax Error\n", 2);
		return ;
	}
	cmds = (t_lstcmds){0, .fd[0][0]=-1, .fd[0][1]=-1, .fd[1][0]=-1,
		.fd[1][1]=-1, 0};
	t = (t_tokens){0};
	while (t.start > -1)
	{
		tokens = token_split(line, &t);
		if (!tokens)
			exit_shell(sh, EXIT_FAILURE, false);
// TODO: PROTECT MALLOC
		cmd = get_command(sh, tokens, cmds.n_cmds);
		cmds.n_cmds++;
		if (!cmd)
			exit_shell(sh, EXIT_FAILURE, false);
// TODO: PROTECT MALLOC
		ft_lstadd_back(&cmds.cmds, ft_lstnew(cmd));
		free_split(tokens);
	}
	sh->cmds = &cmds;
	run_all_cmds(&cmds, sh);
	free_cmds(&cmds);
	sh->cmds = NULL;
}

int	main(int ac, char **av, char **envp)
{
	t_shell	*sh;
	char	*line;

	(void) ac;
	(void) av;
	sh = init_shell(envp);
	if (!get_history(sh))
		stop_main_error(sh, "Problem getting history", EXIT_FAILURE);
	while (1)
	{
		if (g_sig == SIGINT)
		{
			sh->exit_code = 130;
			g_sig = 0;
		}
		line = readline_fd(sh);
		if (!line)
			exit_shell(sh, EXIT_SUCCESS, true);
		if (line && *line)
			put_history(sh, line);
		command_line(sh, line);
	}
	exit_shell(sh, EXIT_SUCCESS, true);
	return (0);
}
