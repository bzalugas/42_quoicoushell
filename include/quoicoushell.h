/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quoicoushell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 22:48:07 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/06 06:50:17 by jsommet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUOICOUSHELL_H
# define QUOICOUSHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include "libft.h"

typedef struct s_shell
{
	t_list	*env_vars;
	t_list	*local_vars;
	char	*cwd;
}	t_shell;

typedef struct s_var
{
	char	*name;
	char	*value;
}	t_var;

//main.c
void	set_signals(void);
void	init_shell(t_shell *sh, char **envp);

// minishell_utils.c
void	exit_shell(t_shell *sh, int exit_code);
char	*current_dir_name(t_shell *sh, int depth);
char	*build_prompt(t_shell *sh);

// variables.c
t_list	*import_env(t_shell *sh, char **envp);
char	**export_env(t_shell *sh);
t_list	*add_variable(t_shell *sh, char *name, char *value, int env);
int		del_variable(t_shell *sh, char *name);

// variables2.c
t_list	*set_variable(t_shell *sh, char *name, char *new_value);
t_list	*get_variable(t_shell *sh, char	*name);
t_var	*new_variable(char *name, char *value);
void	free_variable(void *var);

// variables_utils.c
char	**split_env_entry(char *entry);

// sighandlers.c
void	sigint_handler(int signum);

// ft_readline.c
char	*ft_readline(char *prompt);

#endif
