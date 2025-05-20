#ifndef LIBFT_H
# define LIBFT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
# include "libft.h"

typedef struct s_parse_state {
	int i;
	int j;
	int k;
	int in_single_quote;
	int in_double_quote;
}	t_parse_state;


void		handle_echo_command(char **args, int arg_count);
char		*unescape_string(const char *src);
char		**prepare_exec_args(char **args, int arg_count);
void		handle_ls_command(char **args, char **envp);
void		handle_cat_command(char **args, char **envp);
void		handle_type_command(const char *input);
void		handle_cd_command(char *path);

/* Utils.c */
int		is_shell_builtin(const char *cmd);
void	system_handler(char *input);
void	print_welcome_banner(void);
void	init_shell(void);
void	prompt(void);

/* InputHandler.c */
void	handle_command(char *input, char **args, int arg_count, char **envp);
ssize_t	get_input(char *buffer, size_t size);

/* lib */

/* Ft_strtok.c */
size_t		ft_strlen(const char *s);
char	*ft_strchr(const char *s, int c);
char		*ft_strtok(char *str, const char *delim);

/* Strdup.c */
char		*ft_strdup(const char *s);

/* Path_join.c */
char		*path_join(const char *dir, const char *file);
char		*ft_free(char **ptr);



#endif