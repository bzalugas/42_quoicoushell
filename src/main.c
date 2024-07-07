/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:45:26 by bazaluga          #+#    #+#             */
/*   Updated: 2024/07/07 03:16:04 by jsommet          ###   ########.fr       */
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

void	command_line(t_shell *sh, char *line)
{
	char	**tokens;
	int		i;

	(void) sh;
	// line = replace_vars(sh, line);
	tokens = token_split(line);
	i = 0;
	while (tokens[i])
		ft_putendl_fd(tokens[i++], 1);
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
