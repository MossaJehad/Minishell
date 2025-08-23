/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:44:48 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/23 18:00:56 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_env_command(char **env)
{
	while (env && *env)
	{
		printf("%s\n", *env++);
	}
}

void	replace_env_var(char **env, int idx, const char *var)
{
	free(env[idx]);
	env[idx] = ft_strdup(var);
}

void	append_env_var(char ***envp, const char *var)
{
	char	**env;
	int		i;
	char	**newenv;

	i = 0;
	env = *envp;
	while (env[i])
		i++;
	newenv = ft_calloc((i + 2), sizeof(char *));
	if (!newenv)
		return ;
	i = 0;
	while (env[i])
	{
		newenv[i] = env[i];
		i++;
	}
	newenv[i] = ft_strdup(var);
	newenv[i + 1] = NULL;
	free(env);
	*envp = newenv;
}

void	add_or_replace_env(char ***envp, const char *var)
{
	char	*eq;
	size_t	keylen;
	char	*name;
	int		idx;

	eq = strchr(var, '=');
	if (eq)
		keylen = (size_t)(eq - var);
	else
		keylen = ft_strlen(var);
	name = ft_strndup(var, keylen);
	if (!name)
		return ;
	idx = find_env_index(*envp, name);
	free(name);
	if (idx >= 0)
		replace_env_var(*envp, idx, var);
	else
		append_env_var(envp, var);
}

int	find_env_index(char **env, const char *name)
{
	size_t	n;
	int		i;

	n = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, n) == 0 && env[i][n] == '=')
			return (i);
		i++;
	}
	return (-1);
}
