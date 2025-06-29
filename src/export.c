/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:46:24 by jhaddadi          #+#    #+#             */
/*   Updated: 2025/06/29 19:59:06 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(char **env)
{
	int		i;

	i = 0;
	if (!env)
		return ;
	while (env[i])
		free(env[i++]);
	free(env);
}

void	print_exported_env(char **env)
{
	int		i;
	char	*equal;
	int		var_len;
	char	*var;
	char	*value;

	i = 0;
	while (env[i])
	{
		equal = ft_strchr(env[i], '=');
		if (equal)
		{
			var_len = equal - env[i];
			var = ft_substr(env[i], 0, var_len);
			value = equal + 1;
			printf("declare -x %s=\"%s\"\n", var, value);
			free(var);
		}
		else
		{
			printf("declare -x %s\n", env[i]);
		}
		i++;
	}
}

int	var_exist(char *var, t_data *data)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(var);
	while (data->env[i])
	{
		if (!ft_strncmp(data->env[i], var, len) && data->env[i][len] == '=')
			return (1);
		i++;
	}
	return (0);
}

void	add_to_env(char *var, t_data *data)
{
	int		i;
	char	**new_env;

	i = 0;
	while (data->env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return ;
	i = 0;
	while (data->env[i])
	{
		new_env[i] = ft_strdup(data->env[i]);
		i++;
	}
	new_env[i++] = ft_strdup(var);
	new_env[i] = NULL;
	i = 0;
	while (data->env[i])
	{
		free(data->env[i]);
		i++;
	}
	free(data->env);
	data->env = new_env;
}

int	handle_without_equal(char *arg, t_data *data)
{
	if (!valid_identifier(arg))
	{
		printf("export: %s: not a valid identifier", arg);
		return (1);
	}
	else
	{
		if (!var_exist(arg, data))
			add_to_env(arg, data);
	}
	return (0);
}

void	update_env_var(t_data *data, char *var, char *value)
{
	int		i;
	int		len;
	char	*joined;

	i = 0;
	len = ft_strlen(var);
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], var, len) == 0 && data->env[i][len] == '=')
		{
			free(data->env[i]);
			joined = ft_strjoin(var, "=");
			data->env[i] = ft_strjoin(joined, value);
			free(joined);
			return ;
		}
		i++;
	}
}

int	handle_with_equal(char *arg, t_data *data, char *equal)
{
	int		var_len;
	char	*var;
	char	*value;

	var_len = equal - arg;
	var = ft_substr(arg, 0, var_len);
	value = ft_strdup(equal + 1);
	if (!valid_identifier(var))
	{
		printf("export: %s: not a valid identifier", arg);
		free(var);
		free(value);
		return (1);
	}
	if (var_exist(var, data))
		update_env_var(data, var, value);
	else
		add_to_env(arg, data);
	free(var);
	free(value);
	return (0);
}

void	handle_export_command(char **args, t_data *data)
{
	int		i;
	char	*equal;
	int		error;

	i = 1;
	error = 0;
	if (!args[i])
		print_exported_env(data->env);
	while (args[i])
	{
		equal = ft_strchr(args[i], '=');
		if (equal)
		{
			if (handle_with_equal(args[i], data, equal))
				error = 1;
		}
		else
		{
			if (handle_without_equal(args[i], data))
				error = 1;
		}
		i++;
	}
	
	data->last_status = error;
}
