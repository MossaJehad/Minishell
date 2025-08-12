/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:26 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/12 22:47:15 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*
** ============================================================================
** INCLUDES AND DEFINES
** ============================================================================
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

/* External Libraries */
# include <readline/history.h>
# include <readline/readline.h>

/* Project Libraries */
# include "../libft/libft.h"

/* Project Constants */
# define MAX_ARGS 1024
# define BUFFER_SIZE 4096
# define MAX_COMMANDS 256
# define MAX_PATH_LEN 1024

/* PATH_MAX fallback definition */
# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

/*
** ============================================================================
** STRUCTURES AND TYPES
** ============================================================================
*/

/*
** TOKEN SYSTEM
** Represents parsed command line tokens with type classification
*/
typedef struct s_token
{
	char			*value;		/* Token string value */
	char			*type;		/* Token type (command, word, pipe, redirect, etc.) */
	struct s_token	*next;		/* Next token in linked list */
}					t_token;

/*
** PARSER STATE
** Tracks parsing state for command line processing
*/
typedef struct s_parse_state
{
	int				i;					/* Current character index */
	int				j;					/* Buffer position */
	int				k;					/* Argument count */
	int				in_single_quote;	/* Single quote state flag */
	int				in_double_quote;	/* Double quote state flag */
}					t_parse_state;

/*
** SHELL ENVIRONMENT
** Core shell state and environment variables
*/
typedef struct s_shell
{
	char			*oldpwd;	/* Previous working directory */
	char			*shlvl;		/* Shell level */
	char			*pwd;		/* Current working directory */
}					t_shell;

/*
** EXPANSION CONTEXT
** Holds state for variable expansion in strings
*/
typedef struct s_expand_ctx
{
	int			i;			/* Current index in input string */
	int			j;			/* Current position in output buffer */
	const char 	*arg;		/* Input argument string to expand */
	char		*buffer;	/* Output buffer to write expanded result */
	char		**envp;		/* Environment variables array */
}	t_expand_ctx;

/*
** EXECUTION CONTEXT
** Maintains the state needed to manage command execution, including pipes,
** heredoc file descriptors, process IDs, command count, and command start tokens.
*/
typedef struct s_exec_ctx
{
	int     pipefd[256][2];		/* File descriptors for pipes between commands */
	int     heredoc_fds[256];	/* File descriptors for heredoc inputs */
	pid_t   pids[256];			/* Process IDs of forked child processes */
	int     num_cmds;			/* Number of parsed commands to execute */
	t_token *cmd_starts[256];	/* Array of pointers to the start tokens of each command */
}   t_exec_ctx;


/*
** GLOBAL VARIABLES
** Signal handling and exit status management
*/
extern volatile sig_atomic_t	g_signal;	/* Global signal status */

/*
** ============================================================================
** CORE MODULE - Main shell loop and initialization
** Files: src/core/main.c, src/core/shell_loop.c, src/core/init.c
** ============================================================================
*/

/* Main shell functions */
void				shell_loop(int argc, char ***envp);
char				*get_input(void);
void				init_shell(char **envp);
void				print_welcome_banner(void);

/* Shell state management */
void				null_shell(t_shell *s);
void				init_pwd_vars(char ***envp);
void				init_shlvl(char ***envp);

/*
** ============================================================================
** PARSER MODULE - Input parsing and validation
** Files: src/parser/parsing.c, src/parser/syntax_checker.c, src/parser/quote_handler.c
** ============================================================================
*/

/* Main parsing functions */
char				**parse_arguments(const char *input, int *arg_count);
int					check_syntax_error(char **array);
int					has_unclosed_quotes(const char *input);

/* Parser utilities */
void				null_parse_state(t_parse_state *s);
char				*unescape_string(const char *src);
int					handle_double_char_operators(const char *input,
						t_parse_state *s, char **argv, char *buffer);
int					handle_single_char_operators(const char *input, t_parse_state *s,
						 char **argv, char *buffer);
int					handle_escape_sequences(const char *input, t_parse_state *s,
						 char *buffer);

/* Quote handling */
int					handle_quotes(const char *input, t_parse_state *s, char *buffer);
char				*append_until_quotes_closed(char *line);
int					handle_whitespace(const char *input, t_parse_state *s,
						 char **argv, char *buffer);

/*
** ============================================================================
** TOKENIZER MODULE - Token creation and management
** Files: src/tokenizer/tokenize.c, src/tokenizer/token_utils.c
** ============================================================================
*/

/* Token creation and management */
void				tokenize(char **array, t_token **token);
void				create_token(t_token **token, char *value, char *type);
void				free_tokens(t_token *token);

/* Token classification */
int					check_command(char *word);
int					validate_token(t_token *tok);

/*
** ============================================================================
** BUILTIN COMMANDS MODULE - All shell built-in commands
** Files: src/builtins/echo.c, src/builtins/cd.c, src/builtins/pwd.c,
**        src/builtins/env.c, src/builtins/export.c, src/builtins/unset.c,
**        src/builtins/exit.c
** ============================================================================
*/

/* Built-in command handlers */
void				handle_echo_command(t_token *token);
void				handle_cd_command(char *path, int arg_count, char ***envp);
void				handle_pwd_command(char ***envp);
void				handle_env_command(char **env);
void				handle_export_command(char ***envp, char **args, int arg_count);
void				handle_unset_command(char ***envp, char **args, int arg_count);
void				handle_exit_command(char **args, int arg_count, char **envp);

/* Built-in utilities */
int					is_shell_builtin(const char *cmd);
int					should_run_in_parent(const char *cmd);
char				*determine_cd_target(char *path, char **envp);
int					is_valid_identifier(const char *str);

/* Echo command utilities */
int					count_n_flags(t_token *token);
int					is_all_n(const char *str);

/* Exit command utilities */
void				cleanup_and_exit(int exit_code, char **envp);
int					is_valid_number(const char *str);
int					check_overflow(const char *str);

/*
** ============================================================================
** EXECUTOR MODULE - Command execution and process management
** Files: src/executor/executor.c, src/executor/command_handler.c,
**        src/executor/pipe_handler.c, src/executor/process_manager.c
** ============================================================================
*/

/* Main command execution */
void				handle_command(t_token *token, char ***envp);

/* Process management */
int					parse_commands(t_token *token, t_token *cmd_starts[MAX_COMMANDS], 
								int heredoc_fds[MAX_COMMANDS]);
int					handle_single_command(t_token *cmd_starts[MAX_COMMANDS], 
								int heredoc_fds[MAX_COMMANDS], char ***envp);
int					fork_processes(t_token *cmd_starts[256], int num_cmds, int heredoc_fds[256],
								int pipefd[256][2], pid_t pids[256], char **envp);

/* Process utilities */
void				execute_child_process(t_token *cmd_starts[256], int i, int heredoc_fds[256], 
							int pipefd[256][2], int num_cmds, char **envp);
void				execute_child_builtin(char *cmd_argv[MAX_ARGS], int cmd_argc, char **envp);
void				wait_for_processes(pid_t pids[MAX_COMMANDS], int num_cmds);

/* Pipe management */
int					create_pipes(int pipefd[MAX_COMMANDS][2], int num_cmds);
void				setup_child_pipes(int pipefd[MAX_COMMANDS][2], int i, int num_cmds);
void				close_all_pipes(int pipefd[MAX_COMMANDS][2], int num_cmds);

/* Command building */
int					build_cmd_args(t_token *seg, char *cmd_argv[MAX_ARGS]);
void				handle_single_builtin(char *cmd_argv[MAX_ARGS], int cmd_argc, char ***envp);

/*
** ============================================================================
** SIGNAL HANDLING MODULE - Signal management
** Files: src/signals/signals.c, src/signals/signal_utils.c
** ============================================================================
*/

/* Signal handlers */
void				handle_sigint(int sig);
void				handle_sigquit(int sig);

/* Signal setup functions */
void				setup_signal_handlers(void);
void				setup_child_signals(void);
void				ignore_signals(void);
void				restore_signals(void);

/* Shell status management */
int					get_shell_status(void);
void				set_shell_status(int status);

/*
** ============================================================================
** VARIABLE EXPANSION MODULE - Environment variable and special expansions
** Files: src/expander/expander.c, src/expander/variable_expansion.c,
**        src/expander/quote_expansion.c
** ============================================================================
*/

/* Main expansion function */
char				**expand(char **args, char **envp);

/* Variable expansion */
int					expand_simple_var(t_expand_ctx *ctx);
int					expand_braced_var(t_expand_ctx *ctx);
int					expand_special_vars(t_expand_ctx *ctx);
char				*expand_variables_in_string(char *str, char **envp);
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

/*
** ============================================================================
** I/O AND REDIRECTION MODULE - Input/output handling and redirection
** Files: src/io/input_handler.c, src/io/redirection.c, src/io/heredoc.c
** ============================================================================
*/

/* Input handling */
char				*read_prompt_line(void);

/* Redirection handling */
int					setup_redirection(t_token *token);
int					handle_file_redirection(t_token *tok);
void				setup_child_heredoc(int heredoc_fds[MAX_COMMANDS], int i);

/* Heredoc handling */
int					handle_heredoc(t_token *tok);
int					read_heredoc_lines(int write_fd, const char *delimiter);

/*
** ============================================================================
** UTILITIES MODULE - Helper functions and environment management
** Files: src/utils/utils.c, src/utils/string_utils.c, src/utils/env_utils.c,
**        src/utils/validation.c
** ============================================================================
*/

/* Environment variable management */
char				*lookup_env(const char *name, char **envp);
void				add_or_replace_env(char ***envp, const char *var);
void				add_env_var(char ***envp, const char *var);
void				remove_env_var(char ***envp, const char *name);
int					find_env_index(char **envp, const char *name);
void				replace_env_var(char **env, int idx, const char *var);
void				cleanup_and_exit(int exit_code, char **envp);
void				cleanup_shell_resources(char **env, t_token *token, char **args, char *input);

/* Directory management */
void				update_pwd_oldpwd(char ***envp, const char *new_pwd, 
								const char *old_pwd);

/* Array utilities */
char				**ft_strdup_array(char **array);
void				ft_sort_array(char **array);

/* String utilities - Additional functions not in libft */
int					copy_char(const char *arg, int *i, char *buffer, int j);
char				*ft_strndup(const char *s1, size_t n);
void				free_split(char **split);

/* File descriptor utilities */
void				close_heredoc_fds(int heredoc_fds[MAX_COMMANDS], int num_cmds);

/* Validation utilities */
int					ft_isdigit(int c);

/* Debug and development utilities */
void				prompt(void);

/*
** ============================================================================
** FUNCTION MAPPINGS FOR EXISTING CODE
** These maintain compatibility with current implementation
** ============================================================================
*/

/* Legacy command handlers - to be moved to appropriate modules */
void				handle_cat_command(char **args, char **envp);
void				handle_ls_command(char **args, char **envp);

#endif
