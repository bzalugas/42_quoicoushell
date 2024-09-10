/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quoicoushell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 22:48:07 by jsommet           #+#    #+#             */
/*   Updated: 2024/09/10 18:29:37 by bazaluga         ###   ########.fr       */
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
# ifdef __linux__
#  include <wait.h>
# endif
# define LST_LOCAL 0
# define LST_ENV 1
# define LST_BOTH 2
# define HISTORY_FILE "$HOME/.quoicoushell_history"
// "/home/bazaluga/.quoicoushell_history"
# define HISTORY_MAX_LINES 1000

extern int	g_sigint;

typedef enum e_redir_type
{
	RTIN,
	RTOUT_T,
	RTOUT_A,
}	t_redir_type;

typedef struct s_redir
{
	char			*file;
	t_redir_type	type;
}	t_redir;

typedef struct s_tokens
{
	int	cmd_n;
	int	start;
}	t_tokens;

typedef struct s_var
{
	char	*name;
	char	*value;
}	t_var;

typedef struct s_lstcmds
{
	t_list	*cmds;
	int		fd[2][2];
	int		n_cmds;
}	t_lstcmds;

typedef struct s_cmd
{
	int		n_cmd;
	int		argc;
	char	**argv;
	t_redir	*redirs;
	char	**heredocs;
	char	*hd_filename;
	int		fd_hd;
	bool	heredoc;
}	t_cmd;

typedef struct s_shell
{
	struct sigaction	sa;
	struct sigaction	sa_tmp;
	t_list				*env_vars;
	t_list				*local_vars;
	char				*cwd;
	char				*prompt;
	bool				env_update;
	char				**env;
	char				**paths;
	t_lstcmds			*cmds;
	int					exit_code;
	char				*hist_file;
	t_list				*hist;
	int					n_hist;
}	t_shell;

//main.c
void	init_shell(t_shell *sh, char **envp);
void	command_line(t_shell *sh, char *line);

// minishell_utils.c
void	exit_shell(t_shell *sh, int exit_code, bool display);
char	*current_dir_name(t_shell *sh, int depth);
char	*build_prompt(t_shell *sh);
void	get_history(t_shell *sh);
void	save_history(t_shell *sh);
void	put_history(t_shell *sh, char *line);

// variables.c
t_list	*set_variable(t_shell *sh, char *name, char *value, int where);
int		del_variable(t_shell *sh, char *name);
t_list	*export_variable(t_shell *sh, char *name);

// variables2.c
t_list	*set_variable_value(t_shell *sh, char *name, char *new_value);
t_list	*get_variable(t_shell *sh, char	*name, int where);
t_var	*new_variable(char *name, char *value);
void	free_variable(void *var);
char	*get_variable_value(t_shell *sh, char *name);

// variables_utils.c
char	**split_env_entry(char *entry);

//env_utils.c
t_list	*import_env(t_shell *sh, char **envp);
char	**export_env(t_shell *sh);
char	**export_all_env(t_shell *sh);

// sighandlers.c
void	signal_handler_main(int signum);
void	signal_handler_other(int signum);

// token_split.c
char	**token_split(char *s, t_tokens *t);
int		ft_isquot(char c);
int		ft_isoper(char c);
int		next_quote(char *p);
int		close_par(char *p);
void	remove_quotes(char *word);

// tokenize.c
t_cmd	*get_command(t_shell *sh, char **tokens, int n);

//token_split_utils.c
void	remove_quotes(char *word);

// ft_readline.c
char	*ft_readline(char *prompt);

//exec_handle_streams.c
int		ft_close(t_lstcmds *cmds, int fd);
/* int		get_in_out_files(t_lstcmds *cmds, t_cmd *cmd, bool forked); */
int		get_in_out_files(t_shell *sh, t_cmd *cmd, bool forked);

//exec_heredoc.c
/* int		get_heredocs(t_lstcmds *cmds, t_cmd *cmd); */
int		get_heredocs(t_lstcmds *cmds, t_cmd *cmd, t_shell *sh);

//exec_end_child.c
int		print_perror(char *msg1, char *msg2);
int		print_error(char *msg1, char *msg2);
/* int		stop_perror(char *msg, int error, t_lstcmds *cmds); */
/* int		stop_error(char *msg, int error, t_lstcmds *cmds); */
int	stop_perror(char *msg, int error, t_lstcmds *cmds, t_shell *sh);
int	stop_error(char *msg, int error, t_lstcmds *cmds, t_shell *sh);

//exec_main.c
int		run_all_cmds(t_lstcmds *cmds, t_shell *sh);

//free_cmds.c
void	free_cmd(void *content);
void	free_cmds(t_lstcmds *cmds);

//get_paths.c
char	**get_paths(char **env);

/********************************** BUILTINS **********************************/
int		run_builtin(t_lstcmds *cmds, t_cmd *cmd, t_shell *sh, bool forked);

//export
int		ft_export(t_cmd *cmd, t_shell *sh);
int		ft_local_export(t_cmd *cmd, t_shell *sh);
bool	valid_var_name(char *name);
int		var_error(char *arg);
int		ft_env(t_shell *sh);
int		ft_unset(t_cmd *cmd, t_shell *sh);
int		ft_cd(t_cmd *cmd, t_shell *sh);
int		ft_pwd(t_shell *sh);
int		ft_exit(t_cmd *cmd, t_shell *sh);
int		ft_echo(t_cmd *cmd);

#endif
