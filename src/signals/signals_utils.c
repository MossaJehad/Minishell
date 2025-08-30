/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:41:17 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/30 12:58:45 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	get_shell_status(void)
{
	return (g_signal);
}

void	set_shell_status(int status)
{
	g_signal = status;
}

int	handle_sigint_case(char *line)
{
	dup2(2, 0);
	free(line);
	signal(SIGINT, handle_sigint2);
	return (-1);
}

void	setup_signals2(void)
{
	struct sigaction	sa;

	sa.sa_handler = set_shell_status;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}
