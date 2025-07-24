/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:10 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/07/21 19:38:03 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

 // if the -n followed by n's keep counting tell you find another char
 // (if there is no another char than n then print it as if it 1n else print the whole thing)
void	handle_echo_command(t_token *token)
{
	int		newline;
	t_token	*peek;

	newline = 1;
	token = token->next;
	if (token && ft_strcmp(token->value, "-n") == 0)
	{
		newline = 0;
		token = token->next;
	}
	while (token)
	{
		if (ft_strcmp(token->type, "word") == 0 || ft_strcmp(token->type,
				"command") == 0)
		{
			printf("%s", token->value);
			peek = token->next;
			while (peek && !(ft_strcmp(peek->type, "word") == 0
					|| ft_strcmp(peek->type, "command") == 0))
				peek = peek->next;
			if (peek)
				printf(" ");
		}
		token = token->next;
	}
	if (newline)
		printf("\n");
}

void	handle_env_command(char **env)
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
	if (!path || *path == '\0')
	{
		path = getenv("HOME");
		if (!path)
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
	size_t	n;
	int		i;

	n = ft_strlen(name);
	i = 0;
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
	char	**env;
	char	*eq;
	size_t	keylen;
	char	*name;
	int		idx;
	int		i;
	char	**newenv;
	int		j;

	env = *envp;
	eq = ft_strchr(var, '=');
	keylen = eq ? (size_t)(eq - var) : ft_strlen(var);
	name = ft_strndup(var, keylen);
	idx = find_env_index(env, name);
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
		newenv = malloc(sizeof(char *) * (i + 2));
		j = 0;
		while (j < i)
		{
			newenv[j] = env[j];
			j++;
		}
		newenv[i] = ft_strdup(var);
		newenv[i + 1] = NULL;
		*envp = newenv;
		//free(newenv);
	}
}

void	handle_export_command(char ***envp, char **args, int arg_count)
{
	int		i;
	char	*name;
	char	*eq;
	char	*key;
	char	*value;
	char	*temp_key;
	char	*temp_value;
	size_t	keylen;
	int		j;

	if (arg_count == 1)
	{
		// Print all environment variables in sorted order
		char **e = *envp;
		char **sorted_env = ft_strdup_array(e); // Duplicate the environment array
		ft_sort_array(sorted_env); // Sort the array alphabetically
		j = 0;
		while (sorted_env[j])
		{
			eq = ft_strchr(sorted_env[j], '=');
			if (eq)
				printf("declare -x %.*s=\"%s\"\n", (int)(eq - sorted_env[j]), sorted_env[j], eq + 1);
			else
				printf("declare -x %s\n", sorted_env[j]);
			j++;
		}
		ft_free_array(sorted_env); // Free the sorted array
		return;
	}
	i = 1;
	while (i < arg_count)
	{
		name = args[i];
		eq = ft_strchr(name, '=');
		if (eq)
		{
			// Handle variables with values (e.g., VAR=value)
			keylen = eq - name;
			temp_key = ft_strndup(name, keylen);
			key = ft_strtrim(temp_key, "\"");
			free(temp_key);
			temp_value = ft_strdup(eq + 1);
			value = ft_strtrim(temp_value, "\"");
			free(temp_value);
			if (!is_valid_identifier(key))
				printf("export: `%s': not a valid identifier\n", name);
			else
			{
				char *formatted_var = ft_strjoin(key, "=");
				char *final_var = ft_strjoin(formatted_var, value);
				add_or_replace(envp, final_var);
				free(formatted_var);
				free(final_var);
			}
			free(key);
			free(value);
		}
		else
		{
			// Handle variables without values (e.g., VAR)
			temp_key = ft_strtrim(name, "\"");
			if (!is_valid_identifier(temp_key))
				printf("export: `%s': not a valid identifier\n", name);
			else
			{
				char *buf = ft_strjoin(temp_key, "=");
				add_or_replace(envp, buf);
				free(buf);
			}
			free(temp_key);
		}
		i++;
	}
}

int	is_valid_identifier(const char *str)
{
	size_t	i;

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
	char	**env;
	size_t	n;
	int		i;
	int		cnt;

	env = *envp;
	cnt = 0;
	n = ft_strlen(name);
	while (env[cnt++])
		;
	i = -1;
	while (env[i++])
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
			break ;
		}
	}
}

void	handle_unset_command(char ***envp, char **args, int arg_count)
{
	int	i;

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

