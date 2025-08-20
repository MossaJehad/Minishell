/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 03:02:52 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/20 03:08:36 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	tty_handler(t_shell *shell)
{
	if (shell->exec->tty_fd0 != -1)
	{
		if (dup2(shell->exec->tty_fd0, STDIN_FILENO) < 0)
			err_message(shell, "redirection error", NULL, NULL);
		close(shell->exec->tty_fd0);
		shell->exec->tty_fd0 = -1;
	}
	if (shell->exec->tty_fd1 != -1)
	{
		if (dup2(shell->exec->tty_fd1, STDOUT_FILENO) < 0)
			err_message(shell, "redirection error", NULL, NULL);
		close(shell->exec->tty_fd1);
		shell->exec->tty_fd1 = -1;
	}
}

void	err_message(t_shell *shell, char *cmd, char *arg, char *mess)
{
	ft_putstr_fd("minishell: ", 2);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": ", 2);
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		write(2, ": ", 2);
	}
	if (!mess)
		perror("");
	else
		ft_putendl_fd(mess, 2);
	//if (shell->exec->cmd_count > 1)
	//	close_all_pipes(&shell->exec->pipe, shell->exec->cmd_count);
	tty_handler(shell);
	simple_exit(shell, shell->last_status);
}

void	err_message2(char *cmd, char *arg, char *mess)
{
	ft_putstr_fd("minishell: ", 2);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": ", 2);
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		write(2, ": ", 2);
	}
	if (!mess)
		perror("");
	else
		ft_putendl_fd(mess, 2);
}
