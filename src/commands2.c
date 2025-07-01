/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:45:04 by jhaddadi          #+#    #+#             */
/*   Updated: 2025/07/01 20:14:30 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

int	valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if ((!ft_isalpha(str[0]) && str[0] != '\'' && str[0] != '\"') && str[0] != '_')
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_' && str[i] != '\'' && str[i] != '\"')
			return (0);
		i++;
	}
	return (1);
}

int	env_len(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

void	remove_env_var(t_data *data, char *str)
{
	int		i;
	int		j;
	int		len;
	int		size;
	char	**new_env;

	i = 0;
	j = 0;
	len = ft_strlen(str);
	size = env_len(data->env);
	new_env = malloc(sizeof(char *) * (size + 1));
	if (!new_env)
		return ;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], str, len) == 0 && data->env[i][len] == '=')
			i++;
		else
			new_env[j++] = ft_strdup(data->env[i++]);
	}
	new_env[j] = NULL;
	i = 0;
	while (data->env[i])
		free(data->env[i++]);
	data->env = new_env;
	free(data->env);
}

void	handle_unset_command(char **args, t_data *data)
{
	int	i;

	i = 1;
	if (!args[i])
	{
		data->last_status = 0;
		return ;
	}
	while (args[i])
	{
		if (!valid_identifier(args[i]))
		{
			printf("unset: %s: not a valid identifier\n", args[i]);
			data->last_status = 1;
		}
		else
		{
			remove_env_var(data, args[i]);
			data->last_status = 0;
		}
		i++;
	}
}

void	handle_env_command(char **args, t_data *data)
{
	int	i;

	i = 0;
	if (args[1])
	{
		printf("env: too many arguments\n");
		data->last_status = 2;
		return ;
	}
	while (data->env[i])
	{
		if (ft_strchr(data->env[i], '='))
			printf("%s\n", data->env[i]);
		i++;
	}
	data->last_status = 0;
}

int	is_numeric(const char *str)
{
	int		i;

	i = 0;
	if (!str)
		return (0);
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	handle_exit_command(char **args, t_data *data)
{
	int		i;

	printf("exit\n");
	if (!args[1])
		exit(data->last_status);
	if (!is_numeric(args[1]))
	{
		printf("exit: %s: numeric argument required\n", args[1]);
		exit(255);
	}
	if (args[2])
	{
		printf("exit: too many arguments\n");
		data->last_status = 1;
		return ;
	}
	i = ft_atoi(args[1]) % 256;
	exit(i);
}
