#include "./lib/libft.h"

int	is_shell_builtin(const char *cmd)
{
	if (strcmp(cmd, "type") == 0 || strcmp(cmd, "echo") == 0 || strcmp(cmd,
			"exit") == 0 || strcmp(cmd, "pwd") == 0 || strcmp(cmd, "cd") == 0
		|| strcmp(cmd, "export") == 0 || strcmp(cmd, "env") == 0 || strcmp(cmd,
			"unset") == 0)
	{
		printf("%s is a shell builtin\n", cmd);
		return (1);
	}
	return (0);
}

void	system_handler(char *input)
{
	size_t	len;

	len = strlen(input);
	if (input[len - 1] == '\n')
		input[len - 1] = '\0';
	system(input);
}

void	print_welcome_banner(void)
{
	// printf("┏┓┳┏┓┳┳┓┏┓\n");
	// printf("┗┓┃┃┓┃┃┃┣┫\n");
	// printf("┗┛┻┗┛┛ ┗┛┗ .\n");
	// printf("			\n");
}

void	init_shell(void)
{
	setbuf(stdout, NULL);
	print_welcome_banner();
}

void	prompt(void)
{
	// char cwd[1024];
	// if (getcwd(cwd, sizeof(cwd)) != NULL)
	// printf("%s$ ", cwd);
	// else
	printf("$ ");
}
