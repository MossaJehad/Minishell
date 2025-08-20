/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 12:35:41 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/20 02:53:28 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_env(char **envp)
{
	int	i;

	if (!envp)
		return (1);
	i = 0;
	while (envp[i])
	{
		ft_putendl_fd(envp[i], 1);
		i++;
	}
	return (0);
}

int	add_or_replace_env(char ***envp, const char *var)
{
	int		i;
	size_t	len;
	char	**new_env;

	if (!envp || !*envp || !var)
		return (-1);
	len = 0;
	while (var[len] && var[len] != '=')
		len++;
	i = 0;
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], var, len) && (*envp)[i][len] == '=')
		{
			free((*envp)[i]);
			(*envp)[i] = ft_strdup(var);
			return (0);
		}
		i++;
	}
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (-1);
	for (int j = 0; j < i; j++)
		new_env[j] = (*envp)[j];
	new_env[i] = ft_strdup(var);
	new_env[i + 1] = NULL;
	free(*envp);
	*envp = new_env;
	return (0);
}
