/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:42:16 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/10 16:39:11 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	if (ft_strcmp(path, "-") == 0)
		printf("%s\n", target);
	return (target);
}
