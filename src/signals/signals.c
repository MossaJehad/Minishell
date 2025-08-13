/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 12:00:00 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 05:22:00 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	
	g_signal = 1;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigquit(int sig)
{
	(void)sig;
}


void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	/* Setup SIGINT handler */
	ft_memset(&sa_int, 0, sizeof(sa_int));
	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);

	/* Setup SIGQUIT handler */
	ft_memset(&sa_quit, 0, sizeof(sa_quit));
	sa_quit.sa_handler = handle_sigquit;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	setup_child_signals(void)
{
	/* Reset signal handlers to default for child processes */
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

//void	ignore_signals(void)
//{
//	signal(SIGINT, SIG_IGN);
//	signal(SIGQUIT, SIG_IGN);
//}
