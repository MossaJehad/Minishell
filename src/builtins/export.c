/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:47:13 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/29 17:02:08 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

void	handle_existing_variable(char ***envp
	, char *key, char *existing_value)
{
	char	*buf;
	char	*full_var;

	buf = ft_strjoin(key, "=");
	if (!buf)
		return ;
	full_var = ft_strjoin(buf, existing_value);
	if (!full_var)
	{
		free(buf);
		return ;
	}
	add_or_replace(envp, full_var);
	free(buf);
	free(full_var);
}

void	process_export_variable(char ***envp, char *arg)
{
	char	*temp_key;
	char	*existing_value;

	temp_key = ft_strtrim(arg, "\"");
	if (!temp_key)
		return ;
	if (!is_valid_identifier(temp_key))
	{
		set_shell_status(1);
		perror("export: not a valid identifier");
	}
	else
	{
		existing_value = lookup_env_value(temp_key, *envp);
		if (existing_value)
			handle_existing_variable(envp, temp_key, existing_value);
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
