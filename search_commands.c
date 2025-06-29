#include "minishell.h"

char*	search_commands(const char *input)
{
	char	*path_env;
	char	*dir;
	char	*full_path;

	if (is_shell_builtin(input))
		return (NULL);
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
				return (full_path);
			}
			free(full_path);
			dir = ft_strtok(NULL, ":");
		}
		free(path_env);
	}
	return (NULL);
}
