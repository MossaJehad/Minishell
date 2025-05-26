#include "minishell.h"

void	handle_echo_command(t_token *token)
{
	int newline;
	
	newline = 1;
	token = token->next;
	while (token && ft_strcmp(token->value, "-n") == 0)
	{
		newline = 0;
		token = token->next;
	}
	while (token && (ft_strcmp(token->type, "word") == 0 ||
		ft_strcmp(token->type, "command") == 0))
	{
		printf("%s", unescape_string(token->value));
		if (token->next && ft_strcmp(token->next->type, "word") == 0)
			printf(" ");
		token = token->next;
	}
	if (newline)
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

void	handle_cd_command(char *path, int arg_count)
{
	if(arg_count > 2)
	{
		printf("cd: too many arguments\n");
		return ;
	}
	while (*path == ' ' || *path == '\t')
		path++;
	if (*path == '\0' || ft_strcmp(path, "~") == 0)
		path = getenv("HOME");
	if (chdir(path) != 0)
		printf("cd: %s: No such file or directory\n", path);
}
