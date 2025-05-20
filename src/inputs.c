#include "./lib/libft.h"

void	handle_command(char *input, char **args, int arg_count, char **envp)
{
	char	cwd[1024];
	char	*cmd;

	cmd = args[0];
	if (ft_strcmp(cmd, "exit") == 0 && arg_count > 1 && ft_strcmp(args[1], "0") == 0)
		exit(0);
	else if (ft_strcmp(cmd, "echo") == 0)
		handle_echo_command(args, arg_count);
	else if (ft_strcmp(cmd, "clear") == 0)
		system("clear");
	else if (ft_strcmp(cmd, "ls") == 0)
		handle_ls_command(args, envp);
	else if (ft_strcmp(cmd, "cat") == 0)
		handle_cat_command(args, envp);
	else if (ft_strcmp(cmd, "type") == 0 && arg_count > 1)
		handle_type_command(args[1]);
	else if (ft_strcmp(cmd, "pwd") == 0)
		printf("%s\n", getcwd(cwd, sizeof(cwd)));
	else if (ft_strcmp(cmd, "cd") == 0)
		handle_cd_command(input + 2);
	else if (arg_count == 2)
		system_handler(input);
	else
		printf("%s: command not found\n", input);
}
ssize_t	get_input(char *buffer, size_t size)
{
	ssize_t	bytes_read;

	bytes_read = read(STDIN_FILENO, buffer, size - 1);
	if (bytes_read > 0)
	{
		if (buffer[bytes_read - 1] == '\n')
			buffer[bytes_read - 1] = '\0';
		else
			buffer[bytes_read] = '\0';
	}
	return (bytes_read);
}
