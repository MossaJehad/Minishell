/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:47:13 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/10 10:59:19 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	char	*eq;
	size_t	keylen;
	char	*temp_key;
	char	*key;
	char	*temp_value;
	char	*value;
	char	*formatted_var;
	char	*final_var;

	eq = ft_strchr(arg, '=');
	keylen = eq - arg;
	temp_key = ft_strndup(arg, keylen);
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
}
