/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:41:17 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/02 13:45:15 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_sigquit(int sig)
{
	(void)sig;
}

void	restore_signals(void)
{
	setup_signal_handlers();
}

int	get_shell_status(void)
{
	t_shell	g_shell;

	return (g_shell.last_status);
}

void	set_shell_status(int status)
{
	t_shell	g_shell;

	g_shell.last_status = status;
}
