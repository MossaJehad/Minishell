/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:15 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/07/31 15:10:07 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*lookup_env(const char *name, char **envp)
{
	size_t	len;
	int		i;

	len = ft_strlen(name);
	i = 0;
	while (envp && envp[i])
	{
		if (!ft_strncmp(envp[i], name, len) && envp[i][len] == '=')
			return (&envp[i][len + 1]);
		i++;
	}
	return (NULL);
}

char	*expand_dollar(char *arg, char **envp)
{
	char	*var;
	char	*val;
	char	*exit_status_str;
	pid_t	pid;

	var = arg + 1;
	if (!ft_strcmp(arg, "$"))
		return (ft_strdup("$"));
	if (!ft_strcmp(arg, "$$"))
	{
		pid = getpid();
		exit_status_str = malloc(20);
		sprintf(exit_status_str, "%d", pid);
		free(arg);
		return (exit_status_str);
	}
	if (!ft_strcmp(arg, "$?"))
	{
		exit_status_str = malloc(20);
		sprintf(exit_status_str, "%d", get_exit_status());
		free(arg);
		return (exit_status_str);
	}
	val = lookup_env(var, envp);
	free(arg);
	if (val)
		return (ft_strdup(val));
	else
		return (ft_strdup(""));
}

char	*expand_double_quote(char *arg, char **envp)
{
	char	buffer[BUFFER_SIZE];
	int		i;
	int		j;
	char	*val;
	int		k;
	char	var[256];
	char	*exit_status_str;
	pid_t	pid;

	i = 1;
	j = 0;
	while (arg[i] && arg[i] != '"')
	{
		if (arg[i] == '$' && arg[i + 1])
		{
			if (arg[i + 1] == '?')
			{
				exit_status_str = malloc(20);
				sprintf(exit_status_str, "%d", get_exit_status());
				ft_strlcpy(buffer + j, exit_status_str,
					ft_strlen(exit_status_str) + 1);
				j += ft_strlen(exit_status_str);
				free(exit_status_str);
				i += 2;
			}
			else if (arg[i + 1] == '$')
			{
				pid = getpid();
				exit_status_str = malloc(20);
				sprintf(exit_status_str, "%d", pid);
				ft_strlcpy(buffer + j, exit_status_str,
					ft_strlen(exit_status_str) + 1);
				j += ft_strlen(exit_status_str);
				free(exit_status_str);
				i += 2;
			}
			else if (arg[i + 1] == '{')
			{
				k = 0;
				i += 2;
				while (arg[i] && arg[i] != '}')
					var[k++] = arg[i++];
				var[k] = '\0';
				if (arg[i] == '}')
					i++;
				val = lookup_env(var, envp);
				if (val)
				{
					ft_strlcpy(buffer + j, val, ft_strlen(val) + 1);
					j += ft_strlen(val);
				}
			}
			else if (ft_isalnum(arg[i + 1]) || arg[i + 1] == '_')
			{
				k = 0;
				i++;
				while (ft_isalnum(arg[i]) || arg[i] == '_')
					var[k++] = arg[i++];
				var[k] = '\0';
				val = lookup_env(var, envp);
				if (val)
				{
					ft_strlcpy(buffer + j, val, ft_strlen(val) + 1);
					j += ft_strlen(val);
				}
			}
			else
			{
				buffer[j++] = arg[i++];
			}
		}
		else
			buffer[j++] = arg[i++];
	}
	buffer[j] = '\0';
	free(arg);
	return (ft_strdup(buffer));
}

char	*expand_single_quote(char *arg)
{
	char	*str;
	int		i;
	int		j;
	int		len;

	len = ft_strlen(arg);
	i = 1;
	j = 0;
	if (len < 2)
		return (ft_strdup(""));
	str = malloc(len - 1);
	if (!str)
	{
		ft_free(&str);
		return (NULL);
	}
	while (i < len - 1)
	{
		str[j++] = arg[i++];
	}
	str[j] = '\0';
	free(arg);
	return (str);
}

char	**expand(char **args, char **envp)
{
	int		i;
	char	*new;

	i = 0;
	while (args[i])
	{
		if (args[i][0] == '$')
		{
			new = expand_dollar(ft_strdup(args[i]), envp);
			free(args[i]);
			args[i] = new;
		}
		else if (args[i][0] == '\'')
			args[i] = expand_single_quote(args[i]);
		else if (args[i][0] == '"')
			args[i] = expand_double_quote(args[i], envp);
		i++;
	}
	return (args);
}
