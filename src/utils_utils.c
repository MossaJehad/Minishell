/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 13:49:25 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/02 13:50:30 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Global shell variable - should be defined in one source file
t_shell	g_shell;

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
	// Initialize SHLVL first
	init_shlvl(&envp);
	// Initialize PWD/OLDPWD
	init_pwd_vars(&envp);
	// Update global shell structure with current values
	if (g_shell.shlvl)
		free(g_shell.shlvl);
	if (g_shell.pwd)
		free(g_shell.pwd);
	if (g_shell.oldpwd)
		free(g_shell.oldpwd);
	g_shell.shlvl = ft_strdup(lookup_env_value("SHLVL", envp));
	g_shell.pwd = ft_strdup(lookup_env_value("PWD", envp));
	g_shell.oldpwd = ft_strdup(lookup_env_value("OLDPWD", envp));
	g_shell.last_status = 0;
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
	// Get current PWD to save as OLDPWD
	current_pwd = lookup_env_value("PWD", *envp);
	// Set new PWD to current working directory
	pwd_var = ft_strjoin("PWD=", cwd);
	add_or_replace_env(envp, pwd_var);
	// Set OLDPWD to previous PWD (or empty if first time)
	if (current_pwd)
		oldpwd_var = ft_strjoin("OLDPWD=", current_pwd);
	else
		oldpwd_var = ft_strdup("OLDPWD=");
	add_or_replace_env(envp, oldpwd_var);
	free(cwd);
	free(pwd_var);
	free(oldpwd_var);
}

void	update_pwd_vars(char ***envp, const char *new_pwd)
{
	char	*old_pwd;
	char	*pwd_var;
	char	*oldpwd_var;

	// Get current PWD to save as OLDPWD
	old_pwd = lookup_env_value("PWD", *envp);
	// Update PWD
	pwd_var = ft_strjoin("PWD=", new_pwd);
	add_or_replace_env(envp, pwd_var);
	// Update OLDPWD
	if (old_pwd)
		oldpwd_var = ft_strjoin("OLDPWD=", old_pwd);
	else
		oldpwd_var = ft_strdup("OLDPWD=");
	add_or_replace_env(envp, oldpwd_var);
	// Update global shell structure
	if (g_shell.oldpwd)
		free(g_shell.oldpwd);
	if (g_shell.pwd)
		free(g_shell.pwd);
	g_shell.oldpwd = old_pwd ? ft_strdup(old_pwd) : NULL;
	g_shell.pwd = ft_strdup(new_pwd);
	free(pwd_var);
	free(oldpwd_var);
}

void	free_env(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
		free(envp[i++]);
	free(envp);
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

void	add_or_replace_env(char ***envp, const char *var)
{
	char	**env;
	char	*eq;
	size_t	keylen;
	char	*name;
	int		idx;
	int		i;
	char	**newenv;
	int		j;

	if (!envp || !*envp || !var)
		return ;
	env = *envp;
	eq = ft_strchr(var, '=');
	if (eq)
		keylen = (size_t)(eq - var);
	else
		keylen = ft_strlen(var);
	name = ft_strndup(var, keylen);
	if (!name)
		return ;
	idx = find_env_index(env, name);
	free(name);
	if (idx >= 0)
	{
		// Replace existing variable
		free(env[idx]);
		env[idx] = ft_strdup(var);
	}
	else
	{
		// Add new variable
		i = 0;
		while (env[i])
			i++;
		newenv = malloc(sizeof(char *) * (i + 2));
		if (!newenv)
			return ;
		j = 0;
		while (j < i)
		{
			newenv[j] = env[j];
			j++;
		}
		newenv[i] = ft_strdup(var);
		newenv[i + 1] = NULL;
		free(env);
		*envp = newenv;
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	int		temp;
	int		sign;

	sign = (n < 0) ? -1 : 1;
	temp = n * sign;
	len = (n <= 0) ? 1 : 0;
	while (temp > 0)
	{
		temp /= 10;
		len++;
	}
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (n < 0)
		str[0] = '-';
	temp = n * sign;
	while (temp > 0)
	{
		str[--len] = (temp % 10) + '0';
		temp /= 10;
	}
	if (n == 0)
		str[0] = '0';
	return (str);
}

// Function to clean up shell resources
void	cleanup_shell(void)
{
	if (g_shell.shlvl)
	{
		free(g_shell.shlvl);
		g_shell.shlvl = NULL;
	}
	if (g_shell.pwd)
	{
		free(g_shell.pwd);
		g_shell.pwd = NULL;
	}
	if (g_shell.oldpwd)
	{
		free(g_shell.oldpwd);
		g_shell.oldpwd = NULL;
	}
}
