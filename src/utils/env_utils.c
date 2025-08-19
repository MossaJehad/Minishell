/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 20:50:15 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/18 13:22:51 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void copy_env(char **dest, char **src)
{
	int i = 0;
	while (src[i])
	{
		dest[i] = ft_strdup(src[i]);
		i++;
	}
	dest[i] = NULL; 
}

char	*lookup_env_value(const char *name, char **envp)
{
	size_t	len;
	int		i;

	if (!name || !envp)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], name, len) && envp[i][len] == '=')
			return (&envp[i][len + 1]);
		i++;
	}
	return (NULL);
}
