#include "./lib/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// #include "libft.h"
////// LATEST
int		whileloopstring(int i, int j, int len, char *buffer, const char *src, int bufsize, int string)
{
	while (i < len && j < bufsize - 1)
	{
		if (src[i] == '\\' && i + 1 < len)
		{
			if (string && src[i] == 'n')
				buffer[j++] = '\n';
			else if (src[i] == 't')
				buffer[j++] = '\t';
			else if (src[i] == '\\')
				buffer[j++] = '\\';
			else if (src[i] == '"')
				buffer[j++] = '"';
			else if (src[i] == '\'')
				buffer[j++] = '\'';
			else
				buffer[j++] = src[i];
			i++;
		}
		else
			buffer[j++] = src[i++];
	}
	buffer[j] = '\0';
	return j;
}

char	*unescape_string(const char *src)
{
	static char	buffer[1024];
	char		*processed;
	int			i;
	int			j;
	size_t		len;
	int			string;

	processed = buffer;
	i = 0;
	j = 0;
	string = 0;
	len = ft_strlen(src);
	if (len >= 2 && ((src[0] == '\'' && src[len - 1] == '\'') || (src[0] == '"' && src[len - 1] == '"')))
	{
		src++;
		len -= 2;
		string = 1;
	}
	j = whileloopstring(i, j, len, buffer, src, sizeof(buffer), string);
	return (processed);
}


void	count_quotes(const char *input, int *singleq, int *doubleq)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'')
			(*singleq)++;
		else if (input[i] == '"')
			(*doubleq)++;
		i++;
	}
}

char	**parse_arguments(const char *input, int *arg_count, int *quote_error)
{
	static char	*argv[64];
	char		buffer[1024];
	int			in_single_quote;
	int			in_double_quote;
	int			k;
	int			j;
	int			i;
	char		c;

	i = 0;
	j = 0;
	k = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (input[i])
	{
		c = input[i];
		if (c == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			i++;
			continue ;
		}
		if (c == '"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			i++;
			continue ;
		}
		// !in_single_quote && !in_double_quote &&
		if (!in_single_quote && !in_double_quote && (c == ' ' || c == '\t'))
		{
			if (j > 0)
			{
				buffer[j] = '\0';
				argv[k++] = ft_strdup(buffer);
				j = 0;
			}
			i++;
			continue ;
		}
		if (c == '\\' && input[i + 1])
		{
			buffer[j++] = input[i++];
			buffer[j++] = input[i++];
			continue ;
		}
		buffer[j++] = c;
		i++;
	}
	if (j > 0)
	{
		buffer[j] = '\0';
		argv[k++] = ft_strdup(buffer);
	}
	argv[k] = NULL;
	*arg_count = k;
	*quote_error = in_single_quote || in_double_quote;
	return (argv);
}

void	handle_echo_command(char **args, int arg_count)
{
	int	i;

	i = 1;
	while (i < arg_count)
	{
		printf("%s", unescape_string(args[i]));
		if (i != arg_count - 1)
			printf(" ");
		i++;
	}
	printf("\n");
}

void	handle_cat_command(char **args, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		execve("/usr/bin/cat", args, envp);
		perror("execve failed");
		exit(1);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
	}
	else
	{
		perror("fork failed");
	}
}

void	handle_ls_command(char **args, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		execve("/bin/ls", args, envp);
		perror("execve failed");
		exit(1);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
	}
	else
	{
		perror("fork failed");
	}
}

void	handle_type_command(const char *input)
{
	char	*path_env;
	char	*dir;
	char	*full_path;

	if (is_shell_builtin(input))
		return ;
	path_env = ft_strdup(getenv("PATH"));
	if (path_env)
	{
		dir = ft_strtok(path_env, ":");
		while (dir != NULL)
		{
			full_path = path_join(dir, input);
			if (access(full_path, X_OK) == 0)
			{
				printf("%s is %s\n", input, full_path);
				free(path_env);
				return ;
			}
			dir = ft_strtok(NULL, ":");
		}
		free(path_env);
	}
	printf("%s: not found\n", input);
}

void	handle_cd_command(char *path)
{
	while (*path == ' ' || *path == '\t')
		path++;
	if (*path == '\0' || strcmp(path, "~") == 0)
		path = getenv("HOME");
	if (chdir(path) != 0)
		printf("cd: %s: No such file or directory\n", path);
}

void	handle_command(char *input, char **args, int arg_count, char **envp)
{
	char	cwd[1024];
	char	*cmd;

	cmd = args[0];
	if (strcmp(cmd, "exit") == 0 && arg_count > 1 && strcmp(args[1], "0") == 0)
		exit(0);
	else if (strcmp(cmd, "echo") == 0)
		handle_echo_command(args, arg_count);
	else if (strcmp(cmd, "clear") == 0)
		system("clear");
	else if (strcmp(cmd, "ls") == 0)
		handle_ls_command(args, envp);
	else if (strcmp(cmd, "cat") == 0)
		handle_cat_command(args, envp);
	else if (strcmp(cmd, "type") == 0 && arg_count > 1)
		handle_type_command(args[1]);
	else if (strcmp(cmd, "pwd") == 0)
		printf("%s\n", getcwd(cwd, sizeof(cwd)));
	else if (strcmp(cmd, "cd") == 0)
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

void	shell_loop(char **envp)
{
	char	input[1024];
	int		arg_count;
	int		quote_error;
	int		singleq;
	int		doubleq;
	char	**args;

	quote_error = 0;
	singleq = 0;
	doubleq = 0;
	// printf("%i", singleq);
	while (1)
	{
		prompt();
		if (get_input(input, sizeof(input)) <= 0)
		{
			printf("\n");
			break ;
		}
		count_quotes(input, &singleq, &doubleq);
		args = parse_arguments(input, &arg_count, &quote_error);
		if (quote_error)
		{
			printf("Unmatched quote detected!\n");
			continue ;
		}
		if (arg_count == 0)
			continue ;
		handle_command(input, args, arg_count, envp);
	}
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	init_shell();
	shell_loop(envp);
	return (0);
}