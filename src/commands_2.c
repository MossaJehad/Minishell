#include "minishell.h"
#include <stdio.h>

int valid_identifier(char *str)
{
	int i = 0;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int env_len(char **env)
{
	int i = 0;
	while (env && env[i])
		i++;
	return (i);
}

void remove_env_var(t_data *data, char *str)
{
	int		i = 0;
	int		j = 0;
	int		len = ft_strlen(str);
	int		size = env_len(data->env);
	char	**new_env = malloc(sizeof(char *) * (size + 1));

	if (!new_env)
		return;
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
	free(data->env);

	data->env = new_env;
}

void handle_unset_command(char **args, t_data *data)
{
	int i = 1;

	if (!args[i])
	{
		data->last_status = 0;
		return;
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

void handle_env_command(char **args, t_data *data)
{
	int i = 0;

	if (args[1])
	{
		printf("env: too many arguments\n");
		data->last_status = 2;
		return;
	}
	while (data->env[i])
	{
		printf("%s\n", data->env[i]);
		i++;
	}
	data->last_status = 0;
}
