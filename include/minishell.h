/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:26 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/07/21 18:30:26 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define MAX_ARGS 1024
# define BUFFER_SIZE 4096
# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

// Global exit status variable
extern int			g_exit_status;

typedef struct s_token
{
	char			*value;
	char			*type;
	struct s_token	*next;
}					t_token;

typedef struct s_parse_state
{
	int				i;
	int				j;
	int				k;
	int				in_single_quote;
	int				in_double_quote;
}					t_parse_state;
typedef struct s_exit
{
	int				last_status;
}					t_exit;

void				tokenize(char **array, t_token **token);
void				create_token(t_token **token, char *value, char *type);
int					check_command(char *word);
void				free_tokens(t_token *token);
int					check_syntax_error(char **array);
char				**parse_arguments(const char *input, int *arg_count);
char				*unescape_string(const char *src);
void				shell_loop(int argc, char ***envp);
void				handle_command(t_token *token, char ***envp);
char				*get_input(void);
void				handle_echo_command(t_token *token);
void				handle_env_command(char **env);
void				handle_export_command(char ***envp, char **args,
						int arg_count);
void				handle_cat_command(char **args, char **envp);
void				handle_ls_command(char **args, char **envp);
void				handle_type_command(const char *input);
void				handle_cd_command(char *path, int arg_count);
void				init_shell(char **envp);
int					is_shell_builtin(const char *cmd);
int					should_run_in_parent(const char *cmd);
char				**expand(char **args, char **envp);
void				prompt(void);
void				nully(t_parse_state *s);
int					has_unclosed_quotes(const char *input);
char				*ft_strndup(const char *s1, size_t n);
int					is_valid_identifier(const char *str);
void				handle_unset_command(char ***envp, char **args,
						int arg_count);
void				remove_env_var(char ***envp, const char *name);
int					setup_redirection(t_token *token);
void				free_env(char **envp);
void				print_welcome_banner(void);

/* Add these to your minishell.h file */
// Signal handling includes
# include <limits.h>
# include <signal.h>
# include <sys/wait.h>

// Signal handling functions
void				handle_sigint(int sig);
void				handle_sigquit(int sig);
void				setup_signal_handlers(void);
void				setup_child_signals(void);
void				ignore_signals(void);
void				restore_signals(void);
// Exit status functions
int					get_exit_status(void);
void				set_exit_status(int status);
// Exit command functions
void				handle_exit_command(char **args, int arg_count,
						char **envp);
void				cleanup_and_exit(int exit_code, char **envp);
// Additional utility functions you might need
int					ft_isdigit(int c);
long long			ft_atoll(const char *str);
int					check_overflow(const char *str);
char				**ft_strdup_array(char **array);
void				ft_sort_array(char **array);
void				ft_free_array(char **array);

#endif
