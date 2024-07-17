/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quoicoushell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 22:48:07 by jsommet           #+#    #+#             */
/*   Updated: 2024/07/17 20:34:13 by bazaluga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUOICOUSHELL_H
# define QUOICOUSHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <wait.h>

typedef enum e_redir_type
{
	RTIN,
	RTHEREDOC,
	RTOUT_T,
	RTOUT_A,
}	t_redir_type;

typedef struct s_shell
{
	t_list	*env_vars;
	t_list	*local_vars;
	char	*cwd;
	int		exit_code;
}	t_shell;

typedef struct s_var
{
	char	*name;
	char	*value;
}	t_var;

typedef struct s_redir
{
	char			*file;
	t_redir_type	type;
}	t_redir;

typedef struct s_lstcmds
{
	t_list	*cmds;
	int		fd[2][2];
	int		n_cmds;
	char	**env;
	char	**paths;
}	t_lstcmds;

typedef struct s_cmd
{
	int		n_cmd;
	int		argc;
	char	**argv;
	t_redir	*redirs;
	char	**heredocs;
	bool	heredoc;
}	t_cmd;

//main.c
void	set_signals(void);
void	init_shell(t_shell *sh, char **envp);
void	command_line(t_shell *sh, char *line);

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

//token_split.c
char	**token_split(char *s);
int		ft_isquot(char c);
int		ft_isoper(char c);
int		next_quote(char *p);
int		close_par(char *p);

// ft_readline.c
char	*ft_readline(char *prompt);

//exec_handle_streams.c
int		ft_close(t_lstcmds *cmds, int fd);
int		get_heredoc(t_lstcmds *cmds, t_cmd *cmd);

//exec_end_child.c
int		stop_perror(char *msg, int error, t_lstcmds *cmds);
int		stop_error(char *msg, int error, t_lstcmds *cmds);

//exec_main.c
int		run_all_cmds(t_lstcmds *cmds, t_shell *sh);

//builtins.c
int		run_builtin(t_lstcmds *cmds, t_cmd *cmd, bool forked);

//free_cmds.c
void	free_cmd(void *content);
void	free_cmds(t_lstcmds *cmds);

#endif
