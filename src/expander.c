/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:15 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/07/22 18:08:03 by mhasoneh         ###   ########.fr       */
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

	var = arg + 1;
	if (!ft_strcmp(arg, "$"))
		return (ft_strdup("$"));
	if (!ft_strcmp(arg, "$$") || !ft_strcmp(arg, "$?"))
		return (ft_strdup(arg));
	val = lookup_env(var, envp);
	free(arg);
	return (ft_strdup(val ? val : ""));
}

char	*expand_double_quote(char *arg, char **envp)
{
	char	buffer[BUFFER_SIZE];
	int		i = 1, j;
	char	*val;
	int		k;
			char var[256];

	i = 1, j = 0;
	while (arg[i] && arg[i] != '"')
	{
		if (arg[i] == '$' && arg[i + 1] != '"' && arg[i + 1])
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

	i = 1;
	j = 0;
	len = ft_strlen(arg);
	if (len < 2)
	{
		free(arg);
		return (ft_strdup(""));
	}
	str = malloc(len - 1);
	if (!str)
	{
		free(arg);
		return (NULL);
	}
	while (i < len - 1)
		str[j++] = arg[i++];
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
			new = expand_dollar(args[i], envp);
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
