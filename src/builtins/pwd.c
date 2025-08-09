/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:44:15 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 15:44:30 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

void	handle_pwd_command(char ***envp)
{
	char	*pwd;
	char	cwd[PATH_MAX];

	pwd = lookup_env_value("PWD", *envp);
	if (pwd && access(pwd, F_OK) == 0)
	{
		printf("%s\n", pwd);
		g_signal = 0;
		return ;
	}
	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		update_pwd_oldpwd(envp, cwd, lookup_env_value("PWD", *envp));
		g_signal = 0;
		return ;
	}
	perror("pwd");
	g_signal = 1;
}
