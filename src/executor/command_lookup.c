/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_lookup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 15:28:41 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/29 12:08:21 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_builtin(const char *cmd)
{
	char	*builtins[8];
	int		i;

	builtins[0] = "cd";
	builtins[1] = "echo";
	builtins[2] = "exit";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "pwd";
	builtins[7] = NULL;
	i = 0;
	while (builtins[i])
	{
		if (ft_strcmp(cmd, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	find_in_dirs(char **dirs, char *cmd, int i)
{
	char	*full_path;

	full_path = safe_malloc(i);
	i = 0;
	while (dirs[i])
	{
		ft_strcpy(full_path, dirs[i]);
		ft_strcat(full_path, "/");
		ft_strcat(full_path, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free(full_path);
			free_split(dirs);
			return (1);
		}
		i++;
	}
	free(full_path);
	free_split(dirs);
	return (0);
}

int	find_in_path_str(char *cmd, char **envp, char *path_env)
{
	char	**dirs;
	size_t	max_len;
	size_t	len;
	int		i;

	(void)envp;
	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (0);
	max_len = 0;
	i = 0;
	while (dirs[i])
	{
		len = ft_strlen(dirs[i]);
		if (len > max_len)
			max_len = len;
		i++;
	}
	max_len += 1 + ft_strlen(cmd) + 1;
	return (find_in_dirs(dirs, cmd, max_len));
}

int	resolve_path_env(char *cmd, char **envp)
{
	char	*path_env;
	int		i;

	path_env = NULL;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_env = envp[i] + 5;
			break ;
		}
		i++;
	}
	if (!path_env)
		return (0);
	return (find_in_path_str(cmd, envp, path_env));
}
