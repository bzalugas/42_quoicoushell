/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:45:26 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/25 20:22:57 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quoicoushell.h"

//TODO: LEARN SIGACTION AND REPLACE SIGNAL WITH SIGACTION
void	set_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	init_shell(t_shell *sh, char **envp)
{
	set_signals();
	sh->local_vars = NULL;
	sh->env_vars = NULL;
	import_env(sh, envp);
	sh->cwd = getcwd(NULL, 0);
	sh->prompt = build_prompt(sh);
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
}

void	print_cmd(t_cmd cmd)
{
	if (cmd.heredocs[0])
	{
		print_split(cmd.heredocs, "heredoc\t", "NONE", "\n");
		dprintf(2, "use heredoc: %s\n", cmd.heredoc ? "yes" : "no");
	}
	while (cmd.redirs && cmd.redirs->file)
	{
		dprintf(2, "file: [%s] redir type: %s\n", cmd.redirs->file, get_redir_type(cmd.redirs->type));
		cmd.redirs++;
	}
	dprintf(2, "argc:\t%d, \nargv: ", cmd.argc);
	print_split(cmd.argv, "\t", "NO ARGUMENTS", "\n");
}

// use for parenthesis (bonus so rn useless pretty much)
// pid_t	as_subshell(t_shell *sh, char *line)
// {
// 	pid_t	pid;

// 	pid = fork();
// 	if (pid < 0)
// 		return (perror("fork"), 0);
// 	if (!pid)
// 	{
// 		command_line(sh, line);
// 	}
// 	return (pid);
// }


void	command_line(t_shell *sh, char *line)
{
	char		**tokens;
	t_tokens	t;
	t_cmd		*cmd;
	t_lstcmds	cmds;

	(void) sh;
	cmds = (t_lstcmds){0, .fd[0][0]=-1, .fd[0][1]=-1, .fd[1][0]=-1,
		.fd[1][1]=-1, 0};
	t = (t_tokens){0};
	while (t.start > -1)
	{
		// printf("\t CMD %d: \n", t.cmd_n);
		tokens = token_split(line, &t);
// TODO: PROTECT MALLOC
		cmd = get_command(sh, tokens, cmds.n_cmds);
		cmds.n_cmds++;
// TODO: PROTECT MALLOC
		/* print_cmd(*cmd); */
		ft_lstadd_back(&cmds.cmds, ft_lstnew(cmd));
		free_split(tokens);
	}
	run_all_cmds(&cmds, sh);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	sh;
	char	*line;

	(void) ac;
	(void) av;
	init_shell(&sh, envp);
	while (1)
	{
		line = readline(sh.prompt);
		if (!line)
			exit_shell(&sh, EXIT_SUCCESS);
		if (*line)
			add_history(line);
		command_line(&sh, line);
		free(line);
	}
	exit_shell(&sh, EXIT_SUCCESS);
}
