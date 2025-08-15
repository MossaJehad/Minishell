/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:44:48 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/15 17:46:25 by mhasoneh         ###   ########.fr       */
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
	int		j;
	char	**newenv;
	char	*new_var;

	env = *envp;
	i = 0;
	while (env[i])
		i++;
	newenv = malloc(sizeof(char *) * (i + 2));
	if (!newenv)
		return ;
	j = 0;
	while (j < i)
	{
		newenv[j] = env[j];
		j++;
	}
	new_var = ft_strdup(var);
	if (!new_var)
	{
		free(newenv);
		return ;
	}
	newenv[i] = new_var;
	newenv[i + 1] = NULL;
	free(env);
	*envp = newenv;
	free(newenv);
	free(new_var);
}

void	add_or_replace_env(char ***envp, const char *var)
{
	char	**env;
	char	*eq;
	size_t	keylen;
	char	*name;
	int		idx;

	if (!envp || !*envp || !var)
		return ;
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
