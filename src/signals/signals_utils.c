/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:41:17 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/29 20:11:16 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

int	handle_sigint_case(char *line)
{
	dup2(2, 0);
	free(line);
	signal(SIGINT, handle_sigint);
	return (-1);
}
