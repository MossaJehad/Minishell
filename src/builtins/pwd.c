/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:44:15 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 05:30:21 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_pwd(t_shell *shell, char **args)
{
	char	cwd[PATH_MAX];
	char	*pwd;

	(void)args;

	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		env_add(shell, "PWD", cwd);
		return (EXIT_SUCCESS);
	}

	/* Fallback to PWD environment variable */
	pwd = env_get(shell, "PWD");
	if (pwd)
	{
		printf("%s\n", pwd);
		return (EXIT_SUCCESS);
	}

	perror("pwd");
	return (EXIT_FAILURE);
}

void	update_pwd_oldpwd(char ***envp, const char *new_pwd,
		const char *old_pwd)
{
	char	*pwd_var;
	char	*oldpwd_var;

	pwd_var = ft_strjoin("PWD=", new_pwd);
	if (pwd_var)
	{
		add_or_replace_env(envp, pwd_var);
		free(pwd_var);
	}
	if (old_pwd)
		oldpwd_var = ft_strjoin("OLDPWD=", old_pwd);
	else
		oldpwd_var = ft_strdup("OLDPWD=");
	if (oldpwd_var)
	{
		add_or_replace_env(envp, oldpwd_var);
		free(oldpwd_var);
	}
}
