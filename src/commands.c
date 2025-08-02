/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:10 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/02 14:01:51 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// if the -n followed by n's keep counting tell you find another char
// (if there is no another char than n then
// print it as if it 1n else print the whole thing)
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

void	handle_cd_command(char *path, int arg_count, char ***envp)
{
	char	*target_dir;
	char	*old_pwd;
	char	cwd[PATH_MAX];
	char	*lookup;

	// Check for too many arguments
	if (arg_count > 2)
	{
		fprintf(stderr, "cd: too many arguments\n");
		g_shell.last_status = 1;
		return ;
	}
	// Determine target directory
	target_dir = determine_cd_target(path, *envp);
	if (!target_dir)
	{
		g_shell.last_status = 1;
		return ;
	}
	// Save current PWD as old PWD before changing
	lookup = lookup_env_value("PWD", *envp);
	old_pwd = lookup ? ft_strdup(lookup) : NULL;
	// Change directory
	if (chdir(target_dir) != 0)
	{
		printf("cd: %s: No such file or directory\n", target_dir);
		g_shell.last_status = 1;
		return ;
	}
	// Get the new current working directory
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("cd: getcwd failed");
		g_shell.last_status = 1;
		return ;
	}
	// Update PWD and OLDPWD environment variables
	update_pwd_oldpwd(envp, cwd, old_pwd);
	g_shell.last_status = 0;
}

char	*determine_cd_target(char *path, char **envp)
{
	char	*target;

	// No path or empty path - go to HOME
	if (!path || *path == '\0')
	{
		target = lookup_env_value("HOME", envp);
		if (!target)
		{
			fprintf(stderr, "cd: HOME not set\n");
			return (NULL);
		}
		return (target);
	}
	// Skip leading whitespace
	while (*path == ' ' || *path == '\t')
		path++;
	// Empty after trimming or ~ - go to HOME
	if (*path == '\0' || ft_strcmp(path, "~") == 0)
	{
		target = lookup_env_value("HOME", envp);
		if (!target)
		{
			fprintf(stderr, "cd: HOME not set\n");
			return (NULL);
		}
		return (target);
	}
	// cd - (go to previous directory)
	if (ft_strcmp(path, "-") == 0)
	{
		target = lookup_env_value("OLDPWD", envp);
		if (!target || *target == '\0')
		{
			fprintf(stderr, "cd: OLDPWD not set\n");
			return (NULL);
		}
		printf("%s\n", target); // Print where we're going (bash behavior)
		return (target);
	}
	// Regular path
	return (path);
}

void	update_pwd_oldpwd(char ***envp, const char *new_pwd,
		const char *old_pwd)
{
	char	*pwd_var;
	char	*oldpwd_var;

	// Update PWD
	pwd_var = ft_strjoin("PWD=", new_pwd);
	if (pwd_var)
	{
		add_or_replace_env(envp, pwd_var);
		free(pwd_var);
	}
	// Update OLDPWD
	if (old_pwd)
		oldpwd_var = ft_strjoin("OLDPWD=", old_pwd);
	else
		oldpwd_var = ft_strdup("OLDPWD=");
	if (oldpwd_var)
	{
		add_or_replace_env(envp, oldpwd_var);
		free(oldpwd_var);
	}
	// Update global shell structure
	if (g_shell.pwd)
		free(g_shell.pwd);
	if (g_shell.oldpwd)
		free(g_shell.oldpwd);
	g_shell.pwd = ft_strdup(new_pwd);
	g_shell.oldpwd = old_pwd ? ft_strdup(old_pwd) : NULL;
}

// Simple pwd command implementation
void	handle_pwd_command(char ***envp)
{
	char	*pwd;
	char	cwd[PATH_MAX];

	// Try to get PWD from environment first
	pwd = lookup_env_value("PWD", *envp);
	if (pwd && access(pwd, F_OK) == 0)
	{
		printf("%s\n", pwd);
		g_shell.last_status = 0;
		return ;
	}
	// Fallback to getcwd if PWD is not set or invalid
	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		// Update PWD in environment if it was wrong
		update_pwd_oldpwd(envp, cwd, lookup_env_value("PWD", *envp));
		g_shell.last_status = 0;
		return ;
	}
	perror("pwd");
	g_shell.last_status = 1;
}

int	find_env_index(char **env, const char *name)
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
	if (eq)
		keylen = (size_t)(eq - var);
	else
		keylen = ft_strlen(var);
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
	char	**e;
	char	*formatted_var;
	char	*final_var;
	char	*buf;
	char	**sorted_env;

	e = *envp;
	sorted_env = ft_strdup_array(e);
	if (arg_count == 1)
	{
		ft_sort_array(sorted_env);
		j = 0;
		while (sorted_env[j])
		{
			eq = ft_strchr(sorted_env[j], '=');
			if (eq)
				printf("declare -x %.*s=\"%s\"\n", (int)(eq - sorted_env[j]),
					sorted_env[j], eq + 1);
			else
				printf("declare -x %s\n", sorted_env[j]);
			j++;
		}
		ft_free_array(sorted_env);
		return ;
	}
	i = 1;
	while (i < arg_count)
	{
		name = args[i];
		eq = ft_strchr(name, '=');
		if (eq)
		{
			keylen = eq - name;
			temp_key = ft_strndup(name, keylen);
			key = ft_strtrim(temp_key, "\"");
			free(temp_key);
			temp_value = ft_strdup(eq + 1);
			value = ft_strtrim(temp_value, "\"");
			free(temp_value);
			if (!is_valid_identifier(key))
				perror("export: not a valid identifier");
			else
			{
				formatted_var = ft_strjoin(key, "=");
				final_var = ft_strjoin(formatted_var, value);
				add_or_replace(envp, final_var);
				free(formatted_var);
				free(final_var);
			}
			free(key);
			free(value);
		}
		else
		{
			temp_key = ft_strtrim(name, "\"");
			if (!is_valid_identifier(temp_key))
				perror("export: not a valid identifier");
			else
			{
				buf = ft_strjoin(temp_key, "=");
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
	while (env[cnt])
		cnt++;
	i = 0;
	while (i < cnt)
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
		i++;
	}
}

void	handle_unset_command(char ***envp, char **args, int arg_count)
{
	int	i;

	i = 1;
	while (i < arg_count)
	{
		if (!is_valid_identifier(args[i]))
			perror("unset: not a valid identifier");
		else
			remove_env_var(envp, args[i]);
		i++;
	}
}
