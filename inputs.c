#include "minishell.h"

void	handle_command(char *input, char **args, int arg_count,
						t_token *token)
{
	char	cwd[1024];
	char	*cmd;

	cmd = args[0];
	if (ft_strcmp(cmd, "exit") == 0 && arg_count > 1
		&& ft_strcmp(args[1], "0") == 0)
		exit(0);
	else if (ft_strcmp(cmd, "echo") == 0)
		handle_echo_command(token);
	else if (ft_strcmp(cmd, "type") == 0 && arg_count > 1)
		handle_type_command(args[1]);
	else if (ft_strcmp(cmd, "pwd") == 0)
		printf("%s\n", getcwd(cwd, sizeof(cwd)));
	else if (ft_strcmp(cmd, "cd") == 0)
		handle_cd_command(input + 2, arg_count);
	else
		printf("%s: command not found\n", input);
}

char	*get_input(void)
{
	char	*line;

	line = readline("$ ");
	if (line == NULL)
	{
		printf("\n");
		return (NULL);
	}
	if (line && *line)
		add_history(line);
	return (line);
}
