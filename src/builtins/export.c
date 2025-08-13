/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:47:13 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 05:32:46 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_export(t_shell *shell, char **args)
{
	char	*equals;
	char	*key;
	char	*value;
	int		i;

	if (!args[1])
	{
		/* Print all environment variables in sorted order */
		return (print_exported_vars(shell));
	}

	i = 1;
	while (args[i])
	{
		equals = ft_strchr(args[i], '=');
		if (equals)
		{
			/* Variable assignment */
			key = ft_strndup(args[i], equals - args[i]);
			value = equals + 1;

			if (!is_valid_identifier(key))
			{
				print_error("export", args[i], "not a valid identifier");
				free(key);
				return (EXIT_FAILURE);
			}

			env_add(shell, key, value);
			free(key);
		}
		else
		{
			/* Variable declaration without value */
			if (!is_valid_identifier(args[i]))
			{
				print_error("export", args[i], "not a valid identifier");
				return (EXIT_FAILURE);
			}

			if (!env_exists(shell, args[i]))
				env_add(shell, args[i], NULL);
		}
		i++;
	}

	return (EXIT_SUCCESS);
}

void	export_no_args(char **env)
{
	char	**sorted_env;
	char	*eq;
	int		j;

	sorted_env = ft_strdup_array(env);
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
	ft_free_arr((void *)&sorted_env);
}

void	add_or_replace(char ***envp, const char *var)
{
	char	**env;
	char	*eq;
	size_t	keylen;
	char	*name;
	int		idx;

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

void	process_export_assignment(char ***envp, char *arg)
{
	size_t	keylen;
	char	*temp_key;
	char	*key;
	char	*temp_value;
	char	*value;

	keylen = ft_strchr(arg, '=') - arg;
	temp_key = ft_strndup(arg, keylen);
	key = ft_strtrim(temp_key, "\"");
	free(temp_key);
	temp_value = ft_strdup(ft_strchr(arg, '=') + 1);
	value = ft_strtrim(temp_value, "\"");
	free(temp_value);
	if (!is_valid_identifier(key))
		perror("export: not a valid identifier");
	else
	{
		temp_value = ft_strjoin(key, "=");
		temp_key = ft_strjoin(temp_value, value);
		add_or_replace(envp, temp_key);
		free(temp_value);
		free(temp_key);
	}
	free(key);
	free(value);
}

void	process_export_variable(char ***envp, char *arg)
{
	char	*temp_key;
	char	*buf;

	temp_key = ft_strtrim(arg, "\"");
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

void	handle_export_command(char ***envp, char **args, int arg_count)
{
	int	i;

	if (arg_count == 1)
	{
		export_no_args(*envp);
		return ;
	}
	i = 1;
	while (i < arg_count)
	{
		if (ft_strchr(args[i], '='))
			process_export_assignment(envp, args[i]);
		else
			process_export_variable(envp, args[i]);
		i++;
	}
	g_signal = 0;
}
