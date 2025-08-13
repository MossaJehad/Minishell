/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:29:53 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 04:09:23 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_shell(t_shell *shell, char **envp)
{
	/* Initialize environment from system environment */
	env_init_from_array(shell, envp);
	
	/* Set initial working directory variables */
	init_pwd_vars(shell);
	
	/* Initialize shell level */
	init_shlvl(shell);
	
	/* Set initial exit status */
	shell->exit_status = 0;
	shell->exit_flag = 0;
	
	/* Initialize other fields */
	shell->tokens = NULL;
	shell->commands = NULL;
	shell->pids = NULL;
	shell->num_processes = 0;
	
	/* Print welcome message */
	printf("\033[1;36m");
	printf("╔═══════════════════════════════════════╗\n");
	printf("║            Welcome to Minishell       ║\n");
	printf("║              42 Project               ║\n");
	printf("╚═══════════════════════════════════════╝\n");
	printf("\033[0m\n");
}

void	init_pwd_vars(t_shell *shell)
{
	char	*cwd;
	char	*old_pwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("minishell: getcwd");
		return;
	}
	
	old_pwd = env_get(shell, "PWD");
	if (old_pwd)
		env_add(shell, "OLDPWD", old_pwd);
	
	env_add(shell, "PWD", cwd);
	free(cwd);
}

void	init_shlvl(t_shell *shell)
{
	char	*shlvl_str;
	int		shlvl;
	char	*new_shlvl;

	shlvl_str = env_get(shell, "SHLVL");
	if (shlvl_str)
		shlvl = ft_atoi(shlvl_str) + 1;
	else
		shlvl = 1;
	
	new_shlvl = ft_itoa(shlvl);
	if (new_shlvl)
	{
		env_add(shell, "SHLVL", new_shlvl);
		free(new_shlvl);
	}
}

void	cleanup_shell(t_shell *shell)
{
	if (shell->env)
		env_free(shell->env);
	if (shell->tokens)
		tokens_free(shell->tokens);
	if (shell->commands)
		commands_free(shell->commands);
	if (shell->pids)
		free(shell->pids);
	
	/* Clear readline history */
	rl_clear_history();
}