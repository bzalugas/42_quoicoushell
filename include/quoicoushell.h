/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quoicoushell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsommet <jsommet@student.42.fr >           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 22:48:07 by jsommet           #+#    #+#             */
/*   Updated: 2024/10/08 15:41:41 by bazaluga         ###   ########.fr       */
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
# define HISTORY_MAX_LINES 1000
# define PROMPT_COLOUR "\033[93;1m"
# define PROMPT_END "$\033[0m "
# define C_SQ -1
# define C_DQ -2
# define C_WSP -3
# define CLEAN_FORK 0
# define CLEAN_MAIN 4

extern int	g_sig;

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
	int		idx_in;
	int		idx_out;
	int		argc;
	char	**argv;
	t_redir	*redirs;
	bool	heredoc;
	char	**heredocs;
	char	*hd_file;
	int		fd_hd[2];
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

typedef struct s_cbv
{
	t_cmd	*cmd;
	char	**tks;
	int		arg_i;
	int		tk_i;
	int		hd_i;
	int		rd_i;
}	t_cbv;

typedef struct s_expand_data
{
	char	*full_str;
	char	*tmp_val;
	int		name_size;
	int		new_size;
	char	*new_word;
	int		i;
	int		j;
}	t_expand_data;

// main.c
t_shell	*init_shell(char **envp);
void	command_line(t_shell *sh, char *line);

// signals_setters.c
void	set_signals_main(t_shell *sh);
void	set_exec_parent_signals(t_shell *sh);
void	set_exec_child_signals(t_shell *sh);

// minishell_utils.c
int		exit_shell(t_shell *sh, int exit_code, bool display);
char	*current_dir_name(t_shell *sh, int depth);
char	*build_prompt(t_shell *sh);
char	*readline_fd(t_shell *sh);
int		stop_main_error(t_shell *sh, char *msg, int error);

// history.c
int		get_history(t_shell *sh);
void	save_history(t_shell *sh);
int		put_history(t_shell *sh, char *line);

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

// env_utils.c
t_list	*import_env(t_shell *sh, char **envp);
char	**export_env(t_shell *sh);
char	**export_all_env(t_shell *sh);

// sighandlers.c
void	signal_handler_main(int signum);
void	signal_handler_heredoc(int signum);
void	signal_handler_other(int signum);

//**************** PARSING ******************/
// syntax.c
int		check_syntax(char *line);
// syntax_utils.c
int		increment(char *p);
void	set_expected(int *expected, int a, int b, int c);
int		has_open_quote(char *line);

// expand.c
void	expand(t_shell *sh, char *word, t_expand_data *xdat);
void	replace_quotes(char *word);
void	replace_wsp(char *word);
void	remove_weird_quotes(char *word);
char	*remove_quotes_and_expand(t_shell *sh, char *word);
// expand_utils.c
char	*retrieve_var_name(char *p, t_expand_data *xdat);
int		retrieve_var_value(t_shell *sh, char *p, t_expand_data *xdat);
int		get_new_size(t_shell *sh, char *word, t_expand_data *xdat);
int		split_expand_count(t_shell *sh, char *word);
// expand_utils2.c
int		valid_name_char(char c);
char	active_quote(char *s, int index);
bool	is_valid_tilde(char *p, t_expand_data *xdat);
// expand_fhd.c
char	*expand_fhd(t_shell *sh, char *word);

// parsing.c
t_cmd	*get_command(t_shell *sh, char **tokens, int n);
// parsing_utils.c
void	set_heredoc(t_shell *sh, t_cbv *cbv);
void	set_redir(t_shell *sh, t_cbv *cbv);
void	set_var_assign(t_shell *sh, t_cbv *cbv);
void	split_cpy(char **dest, char **src, size_t src_len);
void	set_cmd_word(t_shell *sh, t_cbv *cbv);

// tokenize.c
bool	can_be_var_assign(char *word);
bool	is_var_assign(t_shell *sh, t_cbv *cbv, char *word);

// token_split.c
char	**token_split(char *s, t_tokens *t);
int		ft_isquot(char c);
int		ft_isoper(char c);
int		next_quote(char *p);
// token_split_utils.c
void	remove_quotes(char *word);

/******************* EXEC *********************/

// exec_handle_streams.c
int		ft_close_all(t_lstcmds *cmds);
int		ft_close(t_lstcmds *cmds, int fd);
int		get_in_out_files(t_shell *sh, t_cmd *cmd, bool forked);
// exec_handle_streams2.c
int		ft_close_all_heredocs(t_lstcmds *cmds, t_cmd *except_cmd);

// exec_heredoc.c
int		get_all_heredocs(t_shell *sh, t_lstcmds *cmds);

// exec_heredoc_utils.
int		clean_heredocs(t_lstcmds *cmds, t_cmd *cmd, int clean_case);
int		eof_ending_heredoc(t_lstcmds *cmds, t_cmd *cmd, char *delim);

// exec_end_child.c
int		print_perror(char *msg1, char *msg2);
int		print_error(char *msg1, char *msg2);
int		stop_perror(char *msg, int error, t_lstcmds *cmds, t_shell *sh);
int		stop_error(char *msg, int error, t_lstcmds *cmds, t_shell *sh);

// exec_main.c
int		run_all_cmds(t_lstcmds *cmds, t_shell *sh);

// free_cmds.c
void	free_cmd(void *content);
void	free_cmds(t_lstcmds *cmds);

// get_paths.c
char	**get_paths(char **env);

/********************************** BUILTINS **********************************/
int		run_builtin(t_lstcmds *cmds, t_cmd *cmd, t_shell *sh, bool forked);

// export
int		ft_export(t_cmd *cmd, t_shell *sh);
int		ft_local_export(t_cmd *cmd, t_shell *sh);
bool	valid_var_name(char *name);
int		var_error(char *arg, char local);
int		ft_env(t_shell *sh);
int		ft_unset(t_cmd *cmd, t_shell *sh);
int		ft_cd(t_cmd *cmd, t_shell *sh);
int		ft_pwd(t_shell *sh);
int		ft_exit(t_cmd *cmd, t_shell *sh);
int		ft_echo(t_shell *sh, t_cmd *cmd);
int		cut_local_exports(t_cmd *cmd, int start_cmd);

#endif
