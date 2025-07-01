/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:46:24 by jhaddadi          #+#    #+#             */
/*   Updated: 2025/07/01 22:23:10 by mhasoneh         ###   ########.fr       */
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
	char	*var;

	var = d_quotes(arg);
	if (!valid_identifier(var))
	{
		printf("export: %s: not a valid identifier", var);
		return (1);
	}
	else
	{
		if (!var_exist(var, data))
			add_to_env(var, data);
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

int	count_quote(char *str)
{
	char	lock;
	int		counter;
	int		i;

	i = 0;
	counter = 0;
	lock = '\0';
	while (str[i])
	{
		if (lock == '\0' && (str[i] == '\"' || str[i] == '\''))
			lock = str[i];
		else if (lock != '\0' && str[i] == lock)
		{
			counter++;
			lock = '\0';
		}
		if (str[i] == lock)
			counter++;
		i++;
	}
	return (counter);
}

char	*d_quotes(char	*str)
{
	char	*result;
	int		num;
	int		num_res;
	char	lock;
	int		count;
	char	l;

	num = 0;
	l = 0;
	num_res = 0;
	count = (ft_strlen(str) - count_quote(str));
	if (count <= 0 || count_quote(str) % 2 == 1)
		return (NULL);
	result = malloc(count + 1);
	lock = '\0';
	while (str[num])
	{
		if (lock == '\0' && (str[num] == '\"' || str[num] == '\''))
			lock = str[num];
		else if (lock != '\0' && str[num] == lock)
		{
			l = 1;
			lock = '\0';
		}
		if (str[num] != lock && !l)
		{
			result[num_res] = str[num];
			num_res++;
		}
		l = 0;
		num++;
	}
	result[num_res] = '\0';
	return (result);
}

int	handle_with_equal(char *arg, t_data *data, char *equal)
{
	int		var_len;
	char	*var;
	char	*var_value;
	char	*value;
	char	*join;
	char	*tmp;

	write (2,"the out\n", 8);
	var_len = equal - arg;
	var_value = ft_substr(arg, 0, var_len);
	var = d_quotes(var_value);
	free(var_value);
	value = d_quotes(equal + 1);
	if (!value)
		return (-1);
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
	{
		if (value)
		{
			tmp = ft_strjoin(var, "=");
			join = ft_strjoin(tmp, value);
			free(tmp);
			add_to_env(join, data);
			free(join);
		}
		else
		{
			add_to_env(var, data);
		}
		printf("export fisrt enterd value\n");
	}
	free(var);
	free(value);
	return (0);
}

void	printf_split(char *str, char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		printf("%s%s\n", str, split[i]);
		i++;
	}
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
