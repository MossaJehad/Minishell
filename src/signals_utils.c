/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:41:17 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/02 14:38:36 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
extern t_shell g_shell;

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
	return (g_shell.last_status);
}

void	set_shell_status(int status)
{
	g_shell.last_status = status;
}
