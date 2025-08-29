/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 12:00:00 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/29 20:09:50 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	g_signal = 130;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigint_child(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	g_signal = 130;
	write(STDOUT_FILENO, "\n", 1);
	rl_done = 1;
}

void	setup_child_signals(void)
{
	signal(SIGINT, handle_sigint_child);
	signal(SIGQUIT, SIG_DFL);
}

void	sig_heredoc(int sig)
{
	printf("\n");
	close(0);
	g_signal = sig;
}
