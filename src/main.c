/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:45:26 by bazaluga          #+#    #+#             */
/*   Updated: 2024/09/17 19:11:08 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

int	g_sigint = 0;

void	set_signals(t_shell *sh)
{
	sh->sa = (struct sigaction) {0};
	sh->sa_tmp = (struct sigaction) {0};
	sh->sa.sa_handler = &signal_handler_main;
	sigaction(SIGINT, &sh->sa, NULL);
	sh->sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sh->sa, NULL);
}

void	init_shell(t_shell *sh, char **envp)
{
	set_signals(sh);
	sh->local_vars = NULL;
	sh->env_vars = NULL;
	import_env(sh, envp);
	sh->cwd = getcwd(NULL, 0);
	sh->prompt = build_prompt(sh);
	sh->env_update = true;
	sh->env = NULL;
	sh->paths = NULL;
	sh->cmds = NULL;
	sh->exit_code = 0;
	set_variable(sh, ft_strdup("SHLVL"),
		ft_itoa(ft_atoi(get_variable_value(sh, "SHLVL")) + 1), LST_ENV);
	sh->hist = NULL;
	sh->hist_file = NULL;
	sh->n_hist = 0;
	sh->stdout_fd = -1;
	sh->tty_fd = -1;
	sh->tty_fd = open("/dev/tty", O_WRONLY);
	if (sh->tty_fd == -1)
		exit_shell(sh, EXIT_FAILURE, false);
	sh->stdout_fd = dup(STDOUT_FILENO);
}

void	print_split(char **sp, char *start, char *none, char *end)
{
	int		i;

	i = 0;
	if (!sp[0])
		dprintf(2, "%s%s%s", start, none, end);
	while (sp[i])
	{
		dprintf(2, "%s[%s] size: %zu%s", start, sp[i], ft_strlen(sp[i]), end);
		i++;
	}
	ft_putchar_fd('\n', 2);

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
// TODO: PROTECT MALLOC
		cmd = get_command(sh, tokens, cmds.n_cmds);
		cmds.n_cmds++;
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
	t_shell	sh;
	char	*line;

	(void) ac;
	(void) av;
	init_shell(&sh, envp);
	get_history(&sh);
	while (1)
	{
		if (g_sigint == 1)
		{
			g_sigint = 0;
			write(1, "\n", 1);
		}
		dup2(sh.tty_fd, STDOUT_FILENO);
		line = readline(sh.prompt);
		dup2(sh.stdout_fd, STDOUT_FILENO);
		if (!line)
			exit_shell(&sh, EXIT_SUCCESS, true);
		if (*line)
			put_history(&sh, line);
		command_line(&sh, line);
	}
	exit_shell(&sh, EXIT_SUCCESS, true);
	return (0);
}
