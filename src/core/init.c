/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:29:53 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 18:31:14 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void print_welcome_banner(void)
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
	t_shell	shell;

	null_shell(&shell);
	init_shlvl(&envp);
	init_pwd_vars(&envp);
	if (shell.shlvl)
		free(shell.shlvl);
	if (shell.pwd)
		free(shell.pwd);
	if (shell.oldpwd)
		free(shell.oldpwd);
	shell.shlvl = ft_strdup(lookup_env_value("SHLVL", envp));
	shell.pwd = ft_strdup(lookup_env_value("PWD", envp));
	shell.oldpwd = ft_strdup(lookup_env_value("OLDPWD", envp));
	g_signal = 0;
	print_welcome_banner();
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
	current_pwd = lookup_env_value("PWD", *envp);
	pwd_var = ft_strjoin("PWD=", cwd);
	add_or_replace_env(envp, pwd_var);
	if (current_pwd)
		oldpwd_var = ft_strjoin("OLDPWD=", current_pwd);
	else
		oldpwd_var = ft_strdup("OLDPWD=");
	add_or_replace_env(envp, oldpwd_var);
	free(cwd);
	free(pwd_var);
	free(oldpwd_var);
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
	shlvl_var = ft_strjoin("SHLVL=", new_shlvl);
	add_or_replace_env(envp, shlvl_var);
	free(new_shlvl);
	free(shlvl_var);
}
