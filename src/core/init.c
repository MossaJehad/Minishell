/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:29:53 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/10 11:05:56 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_welcome_banner(void)
{
	printf("\033[1;97m");
	printf("┏┓┳┏┓┳┳┓┏┓\n");
	printf("\033[1;97m");
	printf("┗┓┃┃┓┃┃┃┣┫\n");
	printf("\033[1;97m");
	printf("┗┛┻┗┛┛ ┗┛┗ .\n");
	printf("\033[0;37m");
	printf("\n");
}

void	init_shell(char **envp)
{
	init_shlvl(&envp);
	init_pwd_vars(&envp);
	g_signal = 0;
	print_welcome_banner();
}

void	init_pwd_vars(char ***envp)
{
	char	*cwd;
	char	*pwd_var;
	char	*oldpwd_var;
	char	*current_pwd;
	char	*current_pwd_copy;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return ;
	current_pwd = lookup_env_value("PWD", *envp);
	current_pwd_copy = current_pwd ? ft_strdup(current_pwd) : NULL;
	pwd_var = ft_strjoin("PWD=", cwd);
	if (pwd_var)
	{
		add_or_replace_env(envp, pwd_var);
		free(pwd_var);
	}
	if (current_pwd_copy)
	{
		oldpwd_var = ft_strjoin("OLDPWD=", current_pwd_copy);
		free(current_pwd_copy);
	}
	else
		oldpwd_var = ft_strdup("OLDPWD=");
	if (oldpwd_var)
	{
		add_or_replace_env(envp, oldpwd_var);
		free(oldpwd_var);
	}
	free(cwd);
}

void	init_shlvl(char ***envp)
{
	char	*shlvl_str;
	int		shlvl;
	char	*new_shlvl;
	char	*shlvl_var;

	shlvl_str = lookup_env_value("SHLVL", *envp);
	if (shlvl_str)
		shlvl = ft_atoi(shlvl_str);
	else
		shlvl = 0;
	shlvl++;
	new_shlvl = ft_itoa(shlvl);
	if (!new_shlvl)
		return ;
	shlvl_var = ft_strjoin("SHLVL=", new_shlvl);
	if (shlvl_var)
	{
		add_or_replace_env(envp, shlvl_var);
		free(shlvl_var);
	}
	free(new_shlvl);
}
