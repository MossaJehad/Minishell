/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malja-fa <malja-fa@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:26 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/30 07:59:28 by malja-fa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*
** ==============================================================
**					INCLUDES AND DEFINES
** ==============================================================
*/

/* Standard Libraries */
# include <fcntl.h>
# include <limits.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
/* External Libraries */
# include <readline/history.h>
# include <readline/readline.h>

/* Project Libraries */
# include "../libft/libft.h"

/* Project Constants */
# define MAX_ARGS 4096
# define BUFFER_SIZE 4096
# define MAX_CMDS 4096
/* PATH_MAX fallback definition */
# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

/*
** ==============================================================
**					STRUCTURES AND TYPES
** ==============================================================
*/
typedef enum e_token_type
{
	WORD,
	COMMAND,
	QUOTED_STRING,
	OPERATOR,
	PIPE,
	REDIRECT,
	REDIRECT_OUT,
	HEREDOC,
	APPEND
}					t_token_type;

typedef struct s_exec_ctx	t_exec_ctx;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
	t_exec_ctx		*ctx;
}					t_token;

/*
** PARSER STATE
** Tracks parsing state for command line processing
*/
typedef struct s_parse_state
{
	int	cursor;			/* Current character index in input */
	int	buffer_pos;		/* Current position in temporary buffer */
	int	arg_count;		/* Number of arguments parsed so far */
	int	in_single_quote;/* Flag: inside single quote */
	int	in_double_quote;/* Flag: inside double quote */
}					t_parse_state;

/*
** SHELL ENVIRONMENT
** Core shell state and environment variables
*/
typedef struct s_shell
{
	char	*pwd;		/* Current working directory */
	char	*oldpwd;	/* Previous working directory */
	int		shlvl;		/* Shell level (as integer for easier increment) */
}					t_shell;

/*
** EXPANSION CONTEXT
** Holds state for variable expansion in strings
*/
typedef struct s_expand_ctx
{
	const char	*input;		/* Input string to expand */
	char		*output;	/* Buffer for expanded result */
	int			in_idx;		/* Current index in input string */
	int			out_idx;	/* Current position in output buffer */
	char		**envp;		/* Environment variables array */
}					t_expand_ctx;

/*
** EXECUTION CONTEXT
** Maintains the state needed to manage command execution, including pipes,
** heredoc file descriptors, process IDs, command count,
	and command start tokens.
*/
typedef struct s_exec_ctx
{
	int		pipe_fds[MAX_CMDS][2];	/* Pipes between commands */
	int		heredoc_fds[MAX_CMDS];	/* Heredoc file descriptors */
	pid_t	pids[MAX_CMDS];			/* Process IDs of child processes */
	int		cmd_count;				/* Number of commands to execute */
	t_token	*cmd_starts[MAX_CMDS];	/* Start token of each command */
}					t_exec_ctx;

typedef struct s_allstructs
{
	/* Core shell state */
	t_shell			*shell;

	/* Input and parsing */
	char			*input;
	t_parse_state	*parse_state;
	t_token			*tokens;

	/* Expansion and execution */
	t_expand_ctx	*expand_ctx;
	t_exec_ctx		*exec_ctx;

	/* Original env */
	char			***envp;
	int				arg_count;
}					t_allstructs;

/*
** GLOBAL VARIABLES
** Signal handling and exit status management
*/
extern volatile \
sig_atomic_t				g_signal;

/*
** ==============================================================
**		CORE MODULE - Main shell loop and initialization
** ==============================================================
*/
/* Main shell functions */
t_token				*prepare_command(char *input, char ***envp, int arg_count);
t_token				*handle_operator(char **args, char **envp);
void				shell_loop(int argc, char ***envp);
void				init_shell(char **envp);
void				print_welcome_banner(void);
/* Shell state management */
void				null_shell(t_shell *s);

/*
** ==============================================================
**		PARSER MODULE - Input parsing and validation
** ==============================================================
*/
/* Main parsing functions */
char				**parse_arguments(const char *input, int *arg_count);
int					check_syntax_error(char **array);

/* Parser utilities */
void				null_parse_state(t_parse_state *s);
int					handle_double_char_operators(const char *input,
						t_parse_state *s, char **argv, char *buffer);
int					handle_single_char_operators(const char *input,
						t_parse_state *s, char **argv, char *buffer);
int					handle_escape_sequences(const char *input, t_parse_state *s,
						char *buffer);

/* Quote handling */
int					handle_quotes(const char *input, t_parse_state *s,
						char *buffer);
char				*append_until_quotes_closed(char *line);
int					handle_whitespace(const char *input, t_parse_state *s,
						char **argv, char *buffer);

/*
** ==============================================================
**		TOKENIZER MODULE - Token creation and management
** ==============================================================
*/

/* Token creation and management */
void				tokenize(char **array, t_token **token);
void				create_token(t_token **token, char *value, char *type);
void				free_tokens(t_token *token);

/* Token classification */
int					check_command(char *word);

int					tokenize_redirects(char **array, int *i, t_token **token);
int					tokenize_pipe_and_redirects(char **array, int *i,
						t_token **token);
void				create_command_or_word_token(char **array, int i,
						t_token **token);
void				validate_tokenize_input(char **array, t_token **token,
						int *should_continue);

/*
** ==============================================================
**		BUILTIN COMMANDS MODULE - All shell built-in commands
** ==============================================================
*/

/* Built-in command handlers */
void				handle_echo_command(t_token *token);
void				handle_cd_command(char *path, int arg_count, char ***envp);
void				handle_env_command(char **env);
void				handle_export_command(char ***envp, char **args,
						int arg_count);
void				handle_unset_command(char ***envp, char **args,
						int arg_count);
void				handle_exit_command(char **args, t_token *seg,
						int arg_count, char ***envp);

/* Built-in utilities */
int					should_run_in_parent(const char *cmd);
char				*determine_cd_target(char *path, char **envp);
int					is_valid_identifier(const char *str);
void				extract_key_value(char *arg, char **key, char **value);
void				assign_env_variable(char ***envp, char *key, char *value);
void				export_no_args(char **env);

/* Echo command utilities */
int					count_n_flags(t_token *token);

/* Exit command utilities */
int					is_valid_number(const char *str);
int					check_overflow(const char *str);

/*
** ==============================================================
**		EXECUTOR MODULE - Command execution and process management
** ==============================================================
*/

/* Main command execution */
void				handle_command(t_token *token, char ***envp);
void				print_ctx(t_exec_ctx *ctx);
/* Process management */
int					parse_commands(t_token *token,
						t_token *cmd_starts[MAX_CMDS],
						int heredoc_fds[MAX_CMDS]);
int					handle_single_command(t_token *cmd_starts[MAX_CMDS],
						int heredoc_fds[MAX_CMDS], char ***envp);
int					fork_processes(t_exec_ctx *ctx, char **envp);

/* Process utilities */
void				execute_child_process(t_exec_ctx *ctx, int i, char **envp);
void				execute_child_builtin(char *cmd_argv[MAX_ARGS],
						int cmd_argc, char **envp);
void				wait_for_processes(pid_t pids[MAX_CMDS], int num_cmds);
int 				is_dir (char *cmd, int flag);

/* Pipe management */
int					create_pipes(int pipefd[MAX_CMDS][2], int num_cmds);
void				setup_child_pipes(int pipefd[MAX_CMDS][2], int i,
						int num_cmds);
void				close_all_pipes(int pipefd[MAX_CMDS][2], int num_cmds);

/* Command building */
int					build_cmd_args(t_token *seg, char *cmd_argv[MAX_ARGS]);
void				handle_single_builtin(char *cmd_argv[MAX_ARGS],
						t_token *seg, int cmd_argc, char ***envp);

/* Parse commands */
void				init_heredoc_fds(int heredoc_fds[MAX_CMDS]);
int					setup_heredocs_for_cmd(t_token *cmd_start, int cmd_index,
						int heredoc_fds[MAX_CMDS]);
int					tokenize_append_and_heredoc(char **array, int *i,
						t_token **token);

/* Helpers */
int					process_token_and_redirects(t_token **cur,
						char *cmd_argv[MAX_ARGS], int *cmd_argc);
void				run_echo_builtin(char *cmd_argv[MAX_ARGS], int cmd_argc);
void				run_pwd_builtin(void);
void				run_env_builtin(char **envp);
int					handle_redirect_tokens(t_token **cur);
int					child_setup_and_collect_args(t_exec_ctx *ctx, int idx,
						char *cmd_argv[MAX_ARGS]);
char				*resolve_cmd(char *cmd, char **envp);
void				skip_heredoc_tokens(t_token **cur);
void				handle_exit_argument(char *arg);
int					count_words_with_quotes(const char *str);
int					is_quoted_expansion(const char *original_arg);
int					count_words_from_split(char *arg);
char				**split_words_with_quotes(const char *str);

/*
** ==============================================================
**		SIGNAL HANDLING MODULE - Signal management
** ==============================================================
*/

/* Signal handlers */
void				handle_sigint(int sig);

/* Signal setup functions */
void				setup_signal_handlers(void);
void				setup_child_signals(void);
void				ignore_signals(void);
void				restore_signals(void);
void				handle_sig_heredoc(int sig);
void				sig_heredoc(int sig);
int					handle_sigint_case(char *line);

/* Shell status management */
int					get_shell_status(void);
void				set_shell_status(int status);
/*
** ==============================================================
**		VARIABLE EXPANSION MODULE - Env variable and special expansions
** ==============================================================
*/

/* Main expansion function */
char				**expand(char **args, char **envp);

/* Variable expansion */
int					expand_simple_var(t_expand_ctx *ctx);
int					expand_braced_var(t_expand_ctx *ctx);
int					expand_special_vars(t_expand_ctx *ctx);
char				*expand_variables_in_string(char *str, char **envp);

/* Quote expansion */
int					process_variable_expansion(t_expand_ctx *ctx);
char				*expand_double_quote(char *arg, char **envp);
char				*expand_single_quote(char *arg);

/* Expansion utilities */
char				*lookup_env_value(const char *name, char **envp);
void				append_env_var(char ***envp, const char *var);
int					handle_dollar_expansion(t_expand_ctx *ctx);
int					expand_exit_status(t_expand_ctx *ctx);
int					expand_pid(t_expand_ctx *ctx);
void				copy_env(char **env, char **envp);

/*
** ==============================================================
**		I/O AND REDIRECTION MODULE - Input/output and redirection
** ==============================================================
*/

/* Redirection handling */
int					setup_redirection(t_token *token);
int					handle_file_redirection(t_token *tok);
void				setup_child_heredoc(int heredoc_fds[MAX_CMDS], int i);

/* Heredoc handling */
int					handle_heredoc(t_token *tok, t_exec_ctx *ctx);
int					read_heredoc_lines(int write_fd, const char *delimiter);

/*
** ==============================================================
**		UTILITIES MODULE - Helper functions and env management
** ==============================================================
*/

/* Environment variable management */
char				*lookup_env(const char *name, char **envp);
void				add_or_replace_env(char ***envp, const char *var);
void				add_or_replace(char ***envp, const char *var);
void				remove_env_var(char ***envp, const char *name);
int					find_env_index(char **envp, const char *name);
void				replace_env_var(char **env, int idx, const char *var);
void				cleanup_shell_resources(char ***env, t_token *token,
						char **args, char *input);
int					is_builtin(const char *cmd);
int					is_blank(const char *s);
char				*trim_whitespace(char *str);
int					build_cmd_args(t_token *seg, char *cmd_argv[MAX_ARGS]);
int					handle_cmd_count_one(t_exec_ctx *ctx, char ***envp);
void				sig_check(int status);

/* Directory management */
void				update_pwd_oldpwd(char ***envp, const char *new_pwd,
						const char *old_pwd);

/* String utilities - Additional functions not in libft */
int					has_unclosed_quotes(const char *input);
char				**apply_word_splitting(char **args, char **envp);
/* File descriptor utilities */
void				close_heredoc_fds(int heredoc_fds[MAX_CMDS],
						int num_cmds);

/* Validation utilities */
int					ft_isdigit(int c);
int					is_operator(const char *s);

/* Debug and development utilities */
void				prompt(void);

#endif