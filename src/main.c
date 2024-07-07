/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:45:26 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/08 01:12:06 by jsommet          ###   ########.fr       */
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
}

void	print_split(char **sp)
{
	int		i;

	i = 0;
	while (sp[i])
	{
		ft_putchar_fd('[', 2);
		ft_putstr_fd(sp[i], 2);
		ft_putstr_fd("] size:", 2);
		ft_putnbr_fd(ft_strlen(sp[i]), 2);
		ft_putchar_fd('\n', 2);
		i++;
	}
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
	char	**tokens;

	(void) sh;
	// line = replace_vars(sh, line);
	tokens = token_split(line);
	print_split(tokens);
	free_split(tokens);
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
		line = readline(build_prompt(&sh));
		if (!line)
			exit_shell(&sh, EXIT_SUCCESS);
		if (*line)
			add_history(line);
		command_line(&sh, line);
		free(line);
	}
	return (0);
}
