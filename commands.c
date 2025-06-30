#include "minishell.h"

void handle_echo_command(t_token *token) {
    int newline;
    
	newline = 1;
    token = token->next;
    if (token && ft_strcmp(token->value, "-n") == 0) {
        newline = 0;
        token = token->next;
    }
    while (token) {
        printf("%s", token->value);
        if (token->next)
            printf(" ");
        token = token->next;
    }
    if (newline)
        printf("\n");
}

void handle_env_command(char **env)
{
    while (env && *env)
    {
        printf("%s\n", *env++);
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
				free(full_path);
				free(path_env);
				return ;
			}
			free(full_path);
			dir = ft_strtok(NULL, ":");
		}
		free(path_env);
	}
	printf("%s: not found\n", input);
}

void	handle_cd_command(char *path, int arg_count)
{
	if (arg_count > 2)
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

static int	find_env_index(char **env, const char *name)
{
    size_t	n = ft_strlen(name);
    int		i = 0;

    while (env[i])
    {
        if (ft_strncmp(env[i], name, n) == 0 && env[i][n] == '=')
            return (i);
        i++;
    }
    return (-1);
}

static void	add_or_replace(char ***envp, const char *var)
{
    char   **env = *envp;
    char    *eq = ft_strchr(var, '=');
    size_t   keylen = eq ? (size_t)(eq - var) : ft_strlen(var);
    char    *name = ft_strndup(var, keylen);
    int      idx = find_env_index(env, name);
    int      i;

    free(name);
    if (idx >= 0)
    {
        free(env[idx]);
        env[idx] = ft_strdup(var);
    }
    else
    {
        i = 0;
        while (env[i])
            i++;
        char **newenv = malloc(sizeof(char *) * (i + 2));
        int j = 0;
        while (j < i)
        {
            newenv[j] = env[j];
            j++;
        }
        newenv[i]     = ft_strdup(var);
        newenv[i + 1] = NULL;
        *envp = newenv;
    }
}

void	handle_export_command(char ***envp, char **args, int arg_count)
{
    char *buf;
    char *name;
    char *eq;
    size_t keylen;
    char *key;
    int i;

    if (arg_count == 1)
    {
        char **e = *envp;
        while (e && *e)
            printf("export %s\n", *e++);
    }
    else
    {
        i = 1;
        while (i < arg_count)
        {
            name = args[i];
            eq = ft_strchr(name, '=');
            if (eq)
            {
                keylen = eq - name;
                key = ft_strndup(name, keylen);
                if (!is_valid_identifier(key))
                    printf("export: `%s': not a valid identifier\n", name);
                else
                    add_or_replace(envp, name);
                free(key);
            }
            else
            {
                if (!is_valid_identifier(name))
                    printf("export: `%s': not a valid identifier\n", name);
                else
                {
                    buf = ft_strjoin(name, "=");
                    add_or_replace(envp, buf);
                    free(buf);
                }
            }
            i++;
        }
    }
}

int	is_valid_identifier(const char *str)
{
    size_t i;

    if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
        return (0);
    i = 1;
    while (str[i])
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

void	remove_env_var(char ***envp, const char *name)
{
    char **env = *envp;
    size_t n;
    int   i, cnt;

    cnt = 0;
    n = ft_strlen(name);
    while (env[cnt++])
        ;
    i = 0;
    while ( env[i])
    {
        if (ft_strncmp(env[i], name, n) == 0 && env[i][n] == '=')
        {
            free(env[i]);
            while (i < cnt - 1)
            {
                env[i] = env[i + 1];
                i++;
            }
            env[i] = NULL;
            break;
        }
        i++;
    }
}

void	handle_unset_command(char ***envp, char **args, int arg_count)
{
    int i;

    i = 1;
    while (i < arg_count)
    {
        if (!is_valid_identifier(args[i]))
            printf("unset: '%s': not a valid identifier\n", args[i]);
        else
            remove_env_var(envp, args[i]);
        i++;
    }
}
