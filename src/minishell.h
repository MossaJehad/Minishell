/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:23:08 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/05/26 17:23:08 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

# define MAX_ARGS 1024
# define BUFFER_SIZE 4096

// ─────────────────────────────────────────────
// Structs
// ─────────────────────────────────────────────

typedef struct s_token
{
	char			*value;
	char			*type;
	struct s_token	*next;
}	t_token;

typedef struct s_parse_state
{
	int	i;
	int	j;
	int	k;
	int	in_single_quote;
	int	in_double_quote;
}	t_parse_state;

typedef struct s_data
{
	char	**env;
	int		last_status;
}	t_data;

// ─────────────────────────────────────────────
// Shell Core
// ─────────────────────────────────────────────

void	init_shell(void);
void	shell_loop(int argc, t_data *data);
char	*get_input(void);

// ─────────────────────────────────────────────
// Argument Parsing & Tokens
// ─────────────────────────────────────────────

char	**parse_arguments(const char *input, int *arg_count, int *quote_error);
void	tokenize(char **array, t_token **token);
void	create_token(t_token **token, char *value, char *type);
int		check_command(char *word);
int		check_syntax_error(char **array, t_data *data);
void	free_tokens(t_token *token);

// ─────────────────────────────────────────────
// Built-in Commands
// ─────────────────────────────────────────────

void	handle_command(char *input, char **args, int arg_count, t_token *token, t_data *data);

void	handle_echo_command(t_token *token);
void	handle_cd_command(char *path, int arg_count, t_data *data);
void	handle_env_command(char **args, t_data *data);
void	handle_unset_command(char **args, t_data *data);
void	handle_export_command(char **args, t_data *data);

void	handle_ls_command(char **args, t_data *data);
void	handle_cat_command(char **args, t_data *data);
void	handle_type_command(const char *input, t_data *data);
int		is_shell_builtin(const char *cmd);
void	system_handler(char *input);

// ─────────────────────────────────────────────
// Export/Unset Helpers
// ─────────────────────────────────────────────

int		valid_identifier(char *str);
int		var_exist(char *var, t_data *data);
void	add_to_env(char *var, t_data *data);
void	update_env_var(t_data *data, char *var, char *value);
void	remove_env_var(t_data *data, char *str);
int		handle_with_equal(char *arg, t_data *data, char *equal);
int		handle_without_equal(char *arg, t_data *data);
void	print_exported_env(char **env);

// ─────────────────────────────────────────────
// Expansion
// ─────────────────────────────────────────────

char	**expand(char **args, t_data *data);
char	*expand_token(char *arg, t_data *data);
char	*expand_dollar(char *arg, int last_status);
char	*expand_double_quote(char *arg, t_data *data);
char	*expand_single_quote(char *arg);
char	*ft_strjoin_free(char *s1, char *s2);

// ─────────────────────────────────────────────
// Utilities
// ─────────────────────────────────────────────

char	**copy_env(char **envp);
void	free_env(char **env);
char	*print_inside_quotes(const char *src);
int		env_len(char **env);
int		whileloopstring(int i, int j, int len, char *buffer,
			const char *src, int bufsize, int string);

#endif
