/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malja-fa <malja-fa@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:06:09 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/30 07:59:43 by malja-fa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_path_env(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

char	*build_full_path(char *dir, char *cmd)
{
	size_t	len;
	char	*full;

	len = ft_strlen(dir) + 1 + ft_strlen(cmd) + 1;
	full = ft_calloc(len, sizeof(char));
	if (!full)
		return (NULL);
	ft_strcpy(full, dir);
	ft_strcat(full, "/");
	ft_strcat(full, cmd);
	return (full);
}

char	*search_paths(char **paths, char *cmd)
{
	int		i;
	char	*full;

	i = 0;
	while (paths && paths[i])
	{
		full = build_full_path(paths[i], cmd);
		if (!full)
			return (NULL);
		if (access(full, X_OK) == 0)
			return (full);
		free(full);
		i++;
	}
	return (NULL);
}

char	*find_executable(char *cmd, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*full;

	path_env = get_path_env(envp);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	full = search_paths(paths, cmd);
	ft_free_arr(paths);
	return (full);
}

char	*resolve_cmd(char *cmd, char **envp)
{
	if (!cmd)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (is_dir(cmd, 0))
			return (NULL);
		return (ft_strdup(cmd));
	}
	return (find_executable(cmd, envp));
}
