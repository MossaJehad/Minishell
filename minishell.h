#ifndef MINISHELL_H
# define MINISHELL_H
# define MAX_ARGS 1024
# define BUFFER_SIZE 4096
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h> 

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

void	tokenize(char **array, t_token **token);
void	create_token(t_token **token, char *value, char *type);
int		check_command(char *word);
void	free_tokens(t_token *token);
int		check_syntax_error(char **array);

char	**parse_arguments(const char *input, int *arg_count, int *quote_error);
char	*unescape_string(const char *src);
int		whileloopstring(int i, int j, int len, char *buffer,
			const char *src, int bufsize, int string);


void	shell_loop(int argc);
void	handle_command(char *input, char **args, int arg_count,
						t_token *token);
char	*get_input(void);

void	handle_echo_command(t_token *token);
void	handle_cat_command(char **args, char **envp);
void	handle_ls_command(char **args, char **envp);
void	handle_type_command(const char *input);
void	handle_cd_command(char *path, int arg_count);


void	init_shell(char **envp);
int		is_shell_builtin(const char *cmd);
char	**expand(char **args);
char	*print_inside_quotes(const char *src);
void	prompt(void);

#endif
