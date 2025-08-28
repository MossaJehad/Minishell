/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:47:13 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/28 08:05:32 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stddef.h>

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
	ft_free_arr(sorted_env);
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
	if (eq)
		keylen = (size_t)(eq - var);
	else
		keylen = ft_strlen(var);
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
	char	*key;
	char	*value;

	extract_key_value(arg, &key, &value);
	assign_env_variable(envp, key, value);
	free(key);
	free(value);
}

void	process_export_variable(char ***envp, char *arg)
{
	char	*temp_key;
	char	*buf;

	temp_key = ft_strtrim(arg, "\"");
	if (!is_valid_identifier(temp_key))
	{
		set_shell_status(1);
		perror("export: not a valid identifier");
	}
	else
	{
		char *existing_value = lookup_env_value(temp_key, *envp);
		if (existing_value)
		{
			buf = ft_strjoin(temp_key, "=");
			char *full_var = ft_strjoin(buf, existing_value);
			add_or_replace(envp, full_var);
			free(buf);
			free(full_var);
		}
		else
			add_or_replace(envp, temp_key);
	}
	free(temp_key);
}

void	handle_export_command(char ***envp, char **args, int arg_count)
{
	int	i;
	int	error_occurred;

	error_occurred = 0;
	if (arg_count == 1)
	{
		export_no_args(*envp);
		g_signal = 0;
		return ;
	}
	i = 1;
	while (i < arg_count)
	{
		if (ft_strchr(args[i], '='))
			process_export_assignment(envp, args[i]);
		else
			process_export_variable(envp, args[i]);
		if (g_signal != 0)
			error_occurred = 1;
		i++;
	}
	if (error_occurred)
		g_signal = 1;
	else
		g_signal = 0;
}
