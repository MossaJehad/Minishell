/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 21:28:30 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/22 00:26:59 by mhasoneh         ###   ########.fr       */
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

void	init_pwd_vars(char ***envp)
{
	char	*cwd;
	char	*pwd_var;
	char	*oldpwd_var;
	char	*current_pwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return ;
	pwd_var = ft_strjoin("PWD=", cwd);
	if (pwd_var)
	{
		add_or_replace_env(envp, pwd_var);
		free(pwd_var);
	}
	current_pwd = lookup_env_value("PWD", *envp);
	if (current_pwd)
		oldpwd_var = ft_strjoin("OLDPWD=", current_pwd);
	else
		oldpwd_var = ft_strdup("OLDPWD=");
	if (oldpwd_var)
	{
		add_or_replace_env(envp, oldpwd_var);
		free(oldpwd_var);
	}
	free(cwd);
}

int	check_shlvl(int shlvl)
{
	if (shlvl > 999)
	{
		printf("minishell: Warning: shell level (%i) \
			too high, resetting to 1\n", shlvl);
		shlvl = 1;
	}
	else if (shlvl < 0)
	{
		printf("minishell: Warning: shell level (%i) too low, resetting to 1\n",
			shlvl);
		shlvl = 1;
	}
	return (shlvl);
}

void	init_shlvl(char ***envp)
{
	char	*shlvl_str;
	int		shlvl;
	char	*new_shlvl;
	char	*shlvl_var;

	shlvl_str = lookup_env_value("SHLVL", *envp);
	if (shlvl_str)
		shlvl = ft_atoi(shlvl_str) + 1;
	else
		shlvl = 1;
	shlvl = check_shlvl(shlvl);
	new_shlvl = ft_itoa(shlvl);
	if (!new_shlvl)
		return ;
	shlvl_var = ft_strjoin("SHLVL=", new_shlvl);
	free(new_shlvl);
	if (shlvl_var)
	{
		add_or_replace_env(envp, shlvl_var);
		free(shlvl_var);
	}
}

void	init_shell(char **envp)
{
	g_signal = 0;
	print_welcome_banner();
	init_shlvl(&envp);
	init_pwd_vars(&envp);
}
