/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:15 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 12:44:09 by mhasoneh         ###   ########.fr       */
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
// return allocated string or NULL if no special case
char *expand_special_dollar(char *arg)
{
	pid_t	pid;
	char	*exit_status_str;

	if (!ft_strcmp(arg, "$"))
		return ft_strdup("$");
	if (!ft_strcmp(arg, "$$"))
	{
		pid = getpid();
		exit_status_str = malloc(20);
		if (!exit_status_str)
			return NULL;
		sprintf(exit_status_str, "%d", pid);
		return exit_status_str;
	}
	if (!ft_strcmp(arg, "$?"))
	{
		exit_status_str = malloc(20);
		if (!exit_status_str)
			return NULL;
		sprintf(exit_status_str, "%d", get_shell_status());
		return exit_status_str;
	}
	return NULL;
}

// Handles $? expansion, returns length written or 0 if no match
int	expand_exit_status(const char *arg, int *i, char *buffer, int j)
{
	char	*exit_status_str;

	if (arg[*i + 1] == '?')
	{
		exit_status_str = malloc(20);
		if (!exit_status_str)
			return (0);
		sprintf(exit_status_str, "%d", get_shell_status());
		ft_strcpy(buffer + j, exit_status_str);
		j += ft_strlen(exit_status_str);
		free(exit_status_str);
		*i += 2;
		return (j);
	}
	return (0);
}

// Handles $$ expansion, returns length written or 0 if no match
int	expand_pid(const char *arg, int *i, char *buffer, int j)
{
	char	*exit_status_str;
	pid_t	pid;

	if (arg[*i + 1] == '$')
	{
		pid = getpid();
		exit_status_str = malloc(20);
		if (!exit_status_str)
			return (0);
		sprintf(exit_status_str, "%d", pid);
		ft_strcpy(buffer + j, exit_status_str);
		j += ft_strlen(exit_status_str);
		free(exit_status_str);
		*i += 2;
		return (j);
	}
	return (0);
}

// Combined function tries both expansions and returns length or 0
static int expand_special_vars(const char *arg, int *i, char *buffer, int j)
{
	int	new_j;

	new_j = expand_exit_status(arg, i, buffer, j);
	if (new_j != 0)
		return (new_j);
	new_j = expand_pid(arg, i, buffer, j);
	if (new_j != 0)
		return (new_j);
	return (0);
}

static int expand_braced_var(const char *arg, int *i, char *buffer, int j, char **envp)
{
	char	var[256];
	int		k;

	k = 0;
	*i += 2;
	while (arg[*i] && arg[*i] != '}' && k < 255)
		var[k++] = arg[(*i)++];
	var[k] = '\0';
	if (arg[*i] == '}')
		(*i)++;
	char *val = lookup_env(var, envp);
	if (val)
	{
		ft_strcpy(buffer + j, val);
		j += ft_strlen(val);
	}
	return (j);
}

static int expand_simple_var(const char *arg, int *i, char *buffer, int j, char **envp)
{
	char	var[256];
	int		k;

	k = 0;
	(*i)++;
	while (arg[*i] && (isalnum(arg[*i]) || arg[*i] == '_') && k < 255)
		var[k++] = arg[(*i)++];
	var[k] = '\0';
	char *val = lookup_env(var, envp);
	if (val)
	{
		strcpy(buffer + j, val);
		j += strlen(val);
	}
	return (j);
}

int	copy_char(const char *arg, int *i, char *buffer, int j)
{
	buffer[j++] = arg[(*i)++];
	return (j);
}

int	handle_dollar_expansion(const char *arg, int *i, char *buffer, int j, char **envp)
{
	int	new_j;

	if (arg[*i] == '$' && arg[*i + 1])
	{
		new_j = expand_special_vars(arg, i, buffer, j);
		if (new_j != 0)
			return (new_j);
		if (arg[*i + 1] == '{')
			return (expand_braced_var(arg, i, buffer, j, envp));
		if (isalnum(arg[*i + 1]) || arg[*i + 1] == '_')
			return (expand_simple_var(arg, i, buffer, j, envp));
		return (copy_char(arg, i, buffer, j));
	}
	return (0);
}

// Core logic processing loop, returns allocated buffer (must be freed by caller)
char *expand_double_quote_loop(char *arg, char **envp)
{
	char	buffer[BUFFER_SIZE];
	int		i;
	int		j;

	i = 1;
	j = 0;
	while (arg[i] && arg[i] != '"')
	{
		int result = handle_dollar_expansion(arg, &i, buffer, j, envp);
		if (result != 0)
		{
			j = result;
			continue;
		}
		j = copy_char(arg, &i, buffer, j);
	}
	buffer[j] = '\0';
	return ft_strdup(buffer);
}

// Public interface: calls loop and manages freeing input
char	*expand_double_quote(char *arg, char **envp)
{
	char	*result;

	result = expand_double_quote_loop(arg, envp);
	free(arg);
	return (result);
}

char	*expand_single_quote(char *arg)
{
	char	*str;
	int		i;
	int		j;

	i = 1;
	j = 0;
	if (ft_strlen(arg) < 2)
		return (ft_strdup(""));
	str = malloc(ft_strlen(arg) - 1);
	if (!str)
	{
		ft_free((void **)&str);
		return (NULL);
	}
	while (i < (int)ft_strlen(arg) - 1)
	{
		str[j++] = arg[i++];
	}
	str[j] = '\0';
	free(arg);
	return (str);
}

int	process_variable_expansion(const char *str, int *i, char *buffer, int j, char **envp)
{
	int	new_j;

	if (str[*i] == '$' && str[*i + 1])
	{
		new_j = expand_exit_status(str, i, buffer, j);
		if (new_j != 0)
			return new_j;
		new_j = expand_pid(str, i, buffer, j);
		if (new_j != 0)
			return new_j;
		if (str[*i + 1] == '{')
			return expand_braced_var(str, i, buffer, j, envp);
		if (isalnum(str[*i + 1]) || str[*i + 1] == '_')
			return expand_simple_var(str, i, buffer, j, envp);
		buffer[j++] = str[(*i)++];
		return (j);
	}
	buffer[j++] = str[(*i)++];
	return (j);
}

char *expand_variables_in_string(char *str, char **envp)
{
	char	buffer[BUFFER_SIZE * 4];
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (str[i])
		j = process_variable_expansion(str, &i, buffer, j, envp);
	buffer[j] = '\0';
	return (ft_strdup(buffer));
}

char	**expand(char **args, char **envp)
{
	int		i;
	char	*new;

	i = 0;
	while (args[i])
	{
		if (args[i][0] == '\'' && args[i][ft_strlen(args[i]) - 1] == '\'')
		{
			args[i] = expand_single_quote(args[i]);
		}
		else if (args[i][0] == '"' && args[i][ft_strlen(args[i]) - 1] == '"')
		{
			args[i] = expand_double_quote(args[i], envp);
		}
		else if (ft_strchr(args[i], '$'))
		{
			new = expand_variables_in_string(args[i], envp);
			free(args[i]);
			args[i] = new;
		}
		i++;
	}
	return (args);
}
