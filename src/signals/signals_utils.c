/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:41:17 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/24 17:02:07 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	return (g_signal);
}

void	set_shell_status(int status)
{
	g_signal = status;
}

void	ignore_signals(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}
