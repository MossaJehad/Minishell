/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_lookup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 15:28:41 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/18 20:24:33 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
* Checks if a given command is a builtin command supported by the shell.
* Builtins include: cd, echo, exit, export, unset, env, and pwd.
*
* @param cmd the command string to check
*
* @return 1 if the command is a builtin, 0 otherwise
*/
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

/**
* Searches for a command inside a list of directories.
* For each directory, it concatenates the directory path with the command name,
* and checks if the resulting path points to an executable file.
*
* @param dirs an array of directory strings (split from PATH)
* @param cmd the command name to search for
* @param i the maximum buffer size required for constructing the full path
*
* @return 1 if the command is found and executable, 0 otherwise
*/
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

/**
* Attempts to find a command inside the PATH string.
* It splits the PATH variable into directories, determines the maximum length
* needed to construct the full command path, then searches each directory.
*
* @param cmd the command name to look for
* @param envp the environment variables (unused in this function)
* @param path_env the value of the PATH environment variable
*
* @return 1 if the command exists in one of the PATH directories, 0 otherwise
*/
int	find_in_path_str(char *cmd, char **envp X_UNUSED, char *path_env)
{
	char	**dirs;
	size_t	max_len;
	size_t	len;
	int		i;

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

/**
* Resolves the PATH environment variable from the environment array.
* If PATH is found, it searches within the directories of PATH for the command.
*
* @param cmd the command name to search for
* @param envp the environment variables
*
* @return 1 if the command is found in PATH directories, 0 otherwise
*/
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

/**
* High-level checker to determine whether a command exists and can be executed.
* The resolution is done in the following order:
* 1. Return false if the command string is NULL or empty.
* 2. Check if the command is a builtin.
* 3. If the command contains '/', check if the path is directly executable.
* 4. Otherwise, attempt to resolve it through the PATH environment variable.
*
* @param cmd the command string to check
* @param envp the environment variables
*
* @return 1 if the command exists and is executable, 0 otherwise
*/
int	command_exists(char *cmd, char **envp)
{
	if (!cmd || *cmd == '\0')
		return (0);
	if (is_builtin(cmd))
		return (1);
	if (ft_strchr(cmd, '/'))
		return (access(cmd, X_OK) == 0);
	return (resolve_path_env(cmd, envp));
}
