/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 13:49:25 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 12:50:58 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_shell_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "type") == 0 || ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "exit") == 0 || ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "unset") == 0)
	{
		return (1);
	}
	return (0);
}

void	null_shell(t_shell *shl)
{
	shl->oldpwd = NULL;
	shl->shlvl = NULL;
	shl->pwd = NULL;
}

int	should_run_in_parent(const char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "exit"));
}

void	print_welcome_banner(void)
{
	printf("┏┓┳┏┓┳┳┓┏┓\n");
	printf("┗┓┃┃┓┃┃┃┣┫\n");
	printf("┗┛┻┗┛┛ ┗┛┗ .\n");
	printf("			\n");
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

char	*lookup_env_value(const char *name, char **envp)
{
	size_t	len;
	int		i;

	if (!name || !envp)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], name, len) && envp[i][len] == '=')
			return (&envp[i][len + 1]);
		i++;
	}
	return (NULL);
}
