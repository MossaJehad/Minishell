/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:42:16 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 05:30:56 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_cd(t_shell *shell, char **args)
{
	char	*target;
	char	*old_pwd;
	char	cwd[PATH_MAX];

	/* Determine target directory */
	if (!args[1] || ft_strcmp(args[1], "~") == 0)
	{
		target = env_get(shell, "HOME");
		if (!target)
		{
			print_error("cd", NULL, "HOME not set");
			return (EXIT_FAILURE);
		}
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		target = env_get(shell, "OLDPWD");
		if (!target)
		{
			print_error("cd", NULL, "OLDPWD not set");
			return (EXIT_FAILURE);
		}
		printf("%s\n", target);
	}
	else
	{
		target = args[1];
	}

	/* Check for too many arguments */
	if (args[1] && args[2])
	{
		print_error("cd", NULL, "too many arguments");
		return (EXIT_FAILURE);
	}

	/* Save current PWD as OLDPWD */
	old_pwd = env_get(shell, "PWD");

	/* Change directory */
	if (chdir(target) != 0)
	{
		perror("cd");
		return (EXIT_FAILURE);
	}

	/* Update PWD and OLDPWD */
	if (getcwd(cwd, sizeof(cwd)))
	{
		env_add(shell, "PWD", cwd);
		if (old_pwd)
			env_add(shell, "OLDPWD", old_pwd);
	}

	return (EXIT_SUCCESS);
}

int	change_directory(char *target_dir, char ***envp)
{
	char	cwd[PATH_MAX];
	char	*lookup;
	char	*old_pwd;

	lookup = lookup_env_value("PWD", *envp);
	old_pwd = lookup ? ft_strdup(lookup) : NULL;
	if (chdir(target_dir) != 0)
	{
		printf("cd: %s: No such file or directory\n", target_dir);
		free(old_pwd);
		return (1);
	}
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("cd: getcwd failed");
		free(old_pwd);
		return (1);
	}
	update_pwd_oldpwd(envp, cwd, old_pwd);
	return (0);
}

void	handle_cd_command(char *path, int arg_count, char ***envp)
{
	char	*target_dir;

	if (arg_count > 2)
	{
		printf("cd: too many arguments\n");
		g_signal = 1;
		return ;
	}
	target_dir = determine_cd_target(path, *envp);
	if (!target_dir)
	{
		g_signal = 1;
		return ;
	}
	g_signal = change_directory(target_dir, envp);
}

char	*resolve_cd_target(char *path, char **envp)
{
	if (!path || *path == '\0')
		return (lookup_env_value("HOME", envp));
	while (*path == ' ' || *path == '\t')
		path++;
	if (*path == '\0' || ft_strcmp(path, "~") == 0)
		return (lookup_env_value("HOME", envp));
	if (ft_strcmp(path, "-") == 0)
		return (lookup_env_value("OLDPWD", envp));
	return (path);
}

char	*determine_cd_target(char *path, char **envp)
{
	char	*target;

	target = resolve_cd_target(path, envp);
	if (!target || *target == '\0')
	{
		if (!path || *path == '\0' || ft_strcmp(path, "~") == 0)
			printf("cd: HOME not set\n");
		else if (ft_strcmp(path, "-") == 0)
			printf("cd: OLDPWD not set\n");
		return (NULL);
	}
	if (path && ft_strcmp(path, "-") == 0)
		printf("%s\n", target);
	return (target);
}
