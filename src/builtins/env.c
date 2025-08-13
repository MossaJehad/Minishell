/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:44:48 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 05:34:16 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_env(t_shell *shell, char **args)
{
	t_env	*current;

	(void)args;

	current = shell->env;
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}

	return (EXIT_SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 00:00:00 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 00:00:00 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*env_new(const char *key, const char *value)
{
	t_env	*env;

	env = (t_env *)safe_malloc(sizeof(t_env));
	env->key = safe_strdup(key);
	env->value = value ? safe_strdup(value) : NULL;
	env->next = NULL;
	return (env);
}

void	env_free(t_env *env)
{
	t_env	*next;

	while (env)
	{
		next = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = next;
	}
}

void	env_add(t_shell *shell, const char *key, const char *value)
{
	t_env	*current;
	t_env	*new_env;

	if (!key)
		return;

	/* Check if key already exists and update it */
	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = value ? safe_strdup(value) : NULL;
			return;
		}
		current = current->next;
	}

	/* Key doesn't exist, create new entry */
	new_env = env_new(key, value);
	new_env->next = shell->env;
	shell->env = new_env;
}

char	*env_get(t_shell *shell, const char *key)
{
	t_env	*current;

	if (!key || !shell->env)
		return (NULL);

	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	env_remove(t_shell *shell, const char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!key || !shell->env)
		return;

	current = shell->env;
	prev = NULL;

	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				shell->env = current->next;
			
			free(current->key);
			free(current->value);
			free(current);
			return;
		}
		prev = current;
		current = current->next;
	}
}

char	**env_to_array(t_shell *shell)
{
	t_env	*current;
	char	**env_array;
	char	*temp;
	int		count;
	int		i;

	/* Count environment variables */
	count = 0;
	current = shell->env;
	while (current)
	{
		count++;
		current = current->next;
	}

	/* Allocate array */
	env_array = allocate_string_array(count);

	/* Fill array */
	current = shell->env;
	i = 0;
	while (current && i < count)
	{
		if (current->value)
		{
			temp = ft_strjoin(current->key, "=");
			env_array[i] = ft_strjoin(temp, current->value);
			free(temp);
		}
		else
		{
			env_array[i] = safe_strdup(current->key);
		}
		current = current->next;
		i++;
	}

	return (env_array);
}

void	env_init_from_array(t_shell *shell, char **envp)
{
	int		i;
	char	*equals;
	char	*key;
	char	*value;

	shell->env = NULL;

	if (!envp)
		return;

	i = 0;
	while (envp[i])
	{
		equals = ft_strchr(envp[i], '=');
		if (equals)
		{
			key = ft_strndup(envp[i], equals - envp[i]);
			value = equals + 1;
			env_add(shell, key, value);
			free(key);
		}
		else
		{
			/* Environment variable without value */
			env_add(shell, envp[i], NULL);
		}
		i++;
	}
}

/*
** Check if environment variable exists
*/
int	env_exists(t_shell *shell, const char *key)
{
	return (env_get(shell, key) != NULL);
}

void	handle_env_command(char **env)
{
	while (env && *env)
	{
		printf("%s\n", *env++);
	}
}

void	replace_env_var(char **env, int idx, const char *var)
{
	free(env[idx]);
	env[idx] = ft_strdup(var);
}

void	append_env_var(char ***envp, const char *var)
{
	char	**env;
	int		i;
	int		j;
	char	**newenv;
	char	*new_var;

	env = *envp;
	i = 0;
	while (env[i])
		i++;
	newenv = malloc(sizeof(char *) * (i + 2));
	if (!newenv)
		return ;
	j = 0;
	while (j < i)
	{
		newenv[j] = env[j];
		j++;
	}
	new_var = ft_strdup(var);
	if (!new_var)
	{
		free(newenv);
		return ;
	}
	newenv[i] = new_var;
	newenv[i + 1] = NULL;
	free(env);
	*envp = newenv;
}

void	add_or_replace_env(char ***envp, const char *var)
{
	char	**env;
	char	*eq;
	size_t	keylen;
	char	*name;
	int		idx;

	if (!envp || !*envp || !var)
		return ;
	env = *envp;
	eq = ft_strchr(var, '=');
	keylen = eq ? (size_t)(eq - var) : ft_strlen(var);
	name = ft_strndup(var, keylen);
	if (!name)
		return ;
	idx = find_env_index(env, name);
	free(name);
	if (idx >= 0)
		replace_env_var(env, idx, var);
	else
		append_env_var(envp, var);
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