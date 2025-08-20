/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:26 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/20 02:59:49 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*
** ============================================================================
**							INCLUDES AND DEFINES
** ============================================================================
*/

/* External Libraries */
# include <readline/history.h>
# include <readline/readline.h>

/* Project Libraries */
# include "../libft/libft.h"

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

/* Internal Header */
# include "./builtins.h"
//# include "parser.h"
//# include "expand.h"
//# include "lexer.h"
//# include "utils.h"
//# include "exec.h"

/* Project Constants */
# define MAX_ARGS 1024
# define BUFFER_SIZE 4096
# define MAX_COMMANDS 256
# define MAX_PATH_LEN 1024
# define MAX_CMDS 256
# define PATH_MAX 4096
# define X_UNUSED __attribute__((unused))


/*
** ============================================================================
** 							STRUCTURES AND TYPES
** ============================================================================
*/


typedef enum e_type
{
	UNKNOWN,
	WORD,
	PIPE,
	SINGLEQ,
	DOUBLEQ,
	DOLLAR,
	REDIRIN,
	REDIROUT,
	APPEND,
	HEREDOC,
	END,
}		t_type;

typedef struct s_token
{
	t_type			type;
	char			*value;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_lexer
{
	t_token	*tokens;
	char	*expand_input;
	int		i;
	int		j;
	int	error;
	int	is_heredoc;
	char	quote;
}	t_lexer;

typedef struct s_clean
{
	void			*ptr;
	struct s_clean	*next;
}			t_clean;

typedef struct s_redir
{
	t_type			type;	/* Type of redirection (enum e_type) */
	char			*file;	/* Target file name or here-doc delimiter */
	struct s_redir	*next;	/* Pointer to next redirection */
	struct s_redir	*prev;	/* Pointer to previous redirection */
}	t_redir;


typedef struct s_cmd
{
	char			**full_cmd;		/* Command with arguments */
	int				is_quote;		/* Flag: command contains quotes */
	int				flag_hd;		/* Flag: heredoc present */
	int				i_hd;			/* Current heredoc index */
	int				hd_count;		/* Total heredocs */
	int				loop_status;	/* Status of command execution in loops */
	int				cmd_len;		/* Number of args in full_cmd */
	int				in_count;		/* Number of input redirections */
	char			*last_file;		/* Last redirection file */
	char			**limiter;		/* Heredoc limiters */
	struct s_redir	*redirs;		/* Linked list of redirections */
	struct s_cmd	*next;			/* Next command in pipeline */
	struct s_cmd	*prev;			/* Previous command in pipeline */
}	t_cmd;


typedef struct s_exec
{
	int		infile;			/* Input file descriptor */
	int		outfile;		/* Output file descriptor */
	int		cmd_count;		/* Number of commands in pipeline */
	int		builtin_less;	/* Flag: pipeline contains no builtins */
	int		pipe[2];		/* Current pipe file descriptors */
	int		old_pipe;		/* Previous pipe (for chaining) */
	int		tty_fd0;		/* Saved stdin for terminal restore */
	int		tty_fd1;		/* Saved stdout for terminal restore */
	int		last_cmd;		/* Index of last command */
	int		if_pipe;		/* Flag: pipeline present */
	pid_t	*pids;			/* Array of child process IDs */
}	t_exec;

typedef struct s_shell
{
	int		argc;		/* Argument count from main() */
	char	**argv;		/* Argument values from main() */
	char	**envp;		/* Environment variables */
	char	*input;		/* Current raw input line */
	int		last_status;/* Exit status of last executed command */
	t_token	*token;		/* Token list from lexer */
	t_cmd	*cmd;		/* Command list after parsing */
	t_exec	*exec;		/* Execution context */
	t_clean	*clean;		/* Cleanup resources tracker */
}	t_shell;

extern volatile sig_atomic_t	g_signal;	/* Global signal status */



void	cleanup_all(t_shell *shell);
char	*ft_getenv(const char *name, char **envp);
char	**arraydup(char **array);
int		count_line(char **array);
void	*tracked_malloc(t_shell *shell, size_t size);
void	err_message(t_shell *shell, char *cmd, char *arg, char *mess);
void	err_message2(char *cmd, char *arg, char *mess);
char	*ft_substr_track(t_shell *shell, char *s, unsigned int start, size_t n);
int		ft_isupper(int c);
int	find_env_var(char **envp, char *var_name);
int	add_or_replace_env(char ***envp, const char *var);






































typedef struct s_parse_state
{
	int	cursor;			/* Current character index in input */
	int	buffer_pos;		/* Current position in temporary buffer */
	int	arg_count;		/* Number of arguments parsed so far */
	int	in_single_quote;/* Flag: inside single quote */
	int	in_double_quote;/* Flag: inside double quote */
}	t_parse_state;

typedef struct s_expand_ctx
{
	const char	*input;	/* Input string to expand */
	char		*output;/* Buffer for expanded result */
	int			in_idx;	/* Current index in input string */
	int			out_idx;/* Current position in output buffer */
	char		**envp;	/* Environment variables array */
}	t_expand_ctx;

typedef struct s_exec_ctx
{
	int		pipe_fds[MAX_CMDS][2];	/* Pipes between commands */
	int		heredoc_fds[MAX_CMDS];	/* Heredoc file descriptors */
	pid_t	pids[MAX_CMDS];			/* Process IDs of child processes */
	int		cmd_count;				/* Number of commands to execute */
	t_token	*cmd_starts[MAX_CMDS];	/* Start token of each command */
}	t_exec_ctx;



/* Main shell functions */
t_token				*prepare_command(char *input, char ***envp, int arg_count);
void				shell_loop(int arg_count, char ***envp, t_shell *shell);
void				init_shell(char **envp);
void				print_welcome_banner(void);

/* Shell state management */
void				null_shell(t_shell *s);

/* Main parsing functions */
char				**parse_arguments(const char *input, int *arg_count);
int					check_syntax_error(char **array);

/* Parser utilities */
void				null_parse_state(t_parse_state *s);
char				*unescape_string(const char *src);
int					handle_double_char_operators(const char *input, t_parse_state *s, char **argv, char *buffer);
int					handle_single_char_operators(const char *input, t_parse_state *s, char **argv, char *buffer);
int					handle_escape_sequences(const char *input, t_parse_state *s, char *buffer);

/* Quote handling */
int					handle_quotes(const char *input, t_parse_state *s, char *buffer);
char				*append_until_quotes_closed(char *line);
int					handle_whitespace(const char *input, t_parse_state *s, char **argv, char *buffer);

/* Token creation and management */
void				tokenize(char **array, t_token **token);
void				create_token(t_token **token, char *value, char *type);
void				free_tokens(t_token *token);

/* Token classification */
int					check_command(char *word);
int					validate_token(t_token *tok);

/* Built-in command handlers */
void				handle_echo_command(t_token *token);
void				handle_cd_command(char *path, int arg_count, char ***envp);
void				handle_pwd_command(char ***envp);
void				handle_env_command(char **env);
void				handle_export_command(char ***envp, char **args, int arg_count);
void				handle_unset_command(char ***envp, char **args, int arg_count);
void				handle_exit_command(char **args, t_token *seg, int arg_count, char ***envp);

/* Built-in utilities */
int					should_run_in_parent(const char *cmd);
char				*determine_cd_target(char *path, char **envp);
int					is_valid_identifier(const char *str);

/* Echo command utilities */
int					count_n_flags(t_token *token);
int					is_all_n(const char *str);

/* Exit command utilities */
void				cleanup_and_exit(int exit_code);
int					is_valid_number(const char *str);
int					check_overflow(const char *str);

/* Main command execution */
void	handle_command(t_token *token, char ***envp, t_shell *shell);
int					command_exists(char *cmd, char **envp);

/* Process management */
int					parse_commands(t_token *token, t_token *cmd_starts[MAX_COMMANDS], int heredoc_fds[MAX_COMMANDS]);
int					handle_single_command(t_token *cmd_starts[256], int heredoc_fds[256],
								char ***envp, t_shell *shell);
int					fork_processes(t_token *cmd_starts[256], int num_cmds, int heredoc_fds[MAX_CMDS],
								int pipefd[256][2], pid_t pids[256], char **envp);

/* Process utilities */
void				execute_child_process(t_token *cmd_starts[256], int i, int heredoc_fds[MAX_CMDS], int pipefd[256][2], int num_cmds, char **envp);
void				execute_child_builtin(char *cmd_argv[MAX_ARGS], int cmd_argc, char **envp);
void				wait_for_processes(pid_t pids[MAX_COMMANDS], int num_cmds);

/* Pipe management */
int					create_pipes(int pipefd[MAX_COMMANDS][2], int num_cmds);
void				setup_child_pipes(int pipefd[MAX_COMMANDS][2], int i, int num_cmds);
void				close_all_pipes(int pipefd[MAX_COMMANDS][2], int num_cmds);

/* Command building */
int					build_cmd_args(t_token *seg, char *cmd_argv[MAX_ARGS]);
void	handle_single_builtin(t_shell *shell, char *cmd_argv[MAX_ARGS],
			int cmd_argc, char ***envp);

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

/* Redirection handling */
int					setup_redirection(t_token *token);
int					handle_file_redirection(t_token *tok);
void				setup_child_heredoc(int heredoc_fds[MAX_COMMANDS], int i);

/* Heredoc handling */
int					handle_heredoc(t_token *tok, t_exec_ctx ctx);
int					read_heredoc_lines(int write_fd, const char *delimiter);

/* Environment variable management */
char				*lookup_env(const char *name, char **envp);
void				update_env_var(t_shell *shell, int i, char *arg);
void				add_env_var(char ***envp, const char *var);
void				remove_env_var(char ***envp, const char *name);
int					find_env_index(char **envp, const char *name);
void				replace_env_var(char **env, int idx, const char *var);
void				cleanup_shell_resources(char ***env, t_token *token, char **args, char *input);
int					is_builtin(const char *cmd);

/* Directory management */
void				update_pwd_oldpwd(char ***envp, const char *new_pwd, const char *old_pwd);

/* String utilities */
int					has_unclosed_quotes(const char *input);

/* File descriptor utilities */
void				close_heredoc_fds(int heredoc_fds[MAX_COMMANDS], int num_cmds);

/* Validation utilities */
int					ft_isdigit(int c);
int					is_operator(const char *s);

/* Debug and development utilities */
void				prompt(void);

/* Legacy command handlers */
void				handle_cat_command(char **args, char **envp);
void				handle_ls_command(char **args, char **envp);

#endif
