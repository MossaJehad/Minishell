/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:48:25 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/20 02:44:09 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_exit(t_shell *shell, char **cmd)
{
	int	code;

	code = 0;
	if (!ft_strcmp(cmd[0], "exit"))
	{
		if (shell->exec->if_pipe == 1)
			exit_code_in_pipes(shell, cmd);
		else
		{
			if (count_line(cmd) > 2)
				exit_err(shell);
			else if (cmd[1] && !cmd[2])
				exit_code(shell, cmd);
		}
		if (!cmd[1])
		{
			close(shell->exec->tty_fd0);
			close(shell->exec->tty_fd1);
			simple_exit(shell, code);
		}
	}
	else
		simple_exit(shell, code);
}

void	simple_exit(t_shell *shell, int code)
{
	code = shell->last_status;
	cleanup_all(shell);
	free(shell->input);
	ft_free_arr(shell->envp);
	free(shell);
	exit(code);
}

void	exit_code(t_shell *shell, char **args)
{
	int	code;

	code = 0;
	if (!is_exit_correct(shell, args[1], 0))
		code = shell->last_status;
	else
		exit_err(shell);
	cleanup_all(shell);
	free(shell->input);
	ft_free_arr(shell->envp);
	free(shell);
	exit(code);
}

void	exit_code_in_pipes(t_shell *shell, char **arg)
{
	int	code;

	code = 0;
	if (count_line(arg) == 1)
		simple_exit(shell, code);
	if (count_line(arg) > 2)
		exit_err(shell);
	else if (!is_exit_correct(shell, arg[1], 0))
		code = ft_atol(arg[1]);
	else
		exit_err(shell);
	code = shell->last_status;
	cleanup_all(shell);
	free(shell->input);
	ft_free_arr(shell->envp);
	free(shell);
	exit(code);
}
