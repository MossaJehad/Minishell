/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 01:08:17 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/20 03:13:03 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"

int	find_env_var(char **envp, char *var_name)
{
	int	i;
	int	var_len;

	if (!envp || !var_name)
		return (-1);
	var_len = ft_strlen(var_name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, var_len) == 0 \
		&& (envp[i][var_len] == '=' || envp[i][var_len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

void	add_to_env(t_shell *shell, char *arg)
{
	int		env_size;
	char	**new_env;

	env_size = 0;
	while (shell->envp && shell->envp[env_size])
		env_size++;
	new_env = ft_realloc_array(shell->envp, env_size + 1);
	if (!new_env)
		return ;
	new_env[env_size] = ft_strdup(arg);
	shell->envp = new_env;
}

void	update_env_var(t_shell *shell, int i, char *arg)
{
	free(shell->envp[i]);
	shell->envp[i] = ft_strdup(arg);
}

int	handle_env_var(t_shell *shell, char *arg)
{
	char	*var_name;
	int		var_index;

	if (!is_valid_var(arg))
	{
		err_message2("export", arg, "not a valid identifier");
		return (1);
	}
	var_name = get_var_name(shell, arg);
	var_index = find_env_var(shell->envp, var_name);
	if (ft_strchr(arg, '='))
	{
		if (var_index >= 0)
			update_env_var(shell, var_index, arg);
		else
			add_to_env(shell, arg);
	}
	return (0);
}

int	ft_export(t_shell *shell)
{
	int	i;
	int	status;

	if (!shell || !shell->cmd || !shell->cmd->full_cmd)
		return (1);
	i = 1;
	status = 0;
	if (!shell->cmd->full_cmd[1])
		ft_env_export(shell->envp);
	else
	{
		while (shell->cmd->full_cmd[i])
		{
			if (shell->cmd->full_cmd[i][0] == '\0')
			{
				i++;
				continue ;
			}
			status = handle_env_var(shell, shell->cmd->full_cmd[i]);
			i++;
		}
	}
	return (status);
}
