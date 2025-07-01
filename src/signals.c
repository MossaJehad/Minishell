/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhaddadi <jhaddadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 13:05:22 by jhaddadi          #+#    #+#             */
/*   Updated: 2025/07/01 17:52:52 by jhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fatal_error(char *msg, char *input,
	char **args, t_token *token, t_data *data)
{
	if (msg)
		ft_putendl_fd(msg, STDERR_FILENO);
	if (input)
		free(input);
	if (args)
		ft_free(args);
	if (token)
		free_tokens(token);
	if (data && data->env)
		free_env(data->env);
	exit(EXIT_FAILURE);
}

void	signal_reset_prompt(int signo)
{
	(void)signo;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	set_signals_interactive(void)
{
	struct sigaction	act;

	act.sa_handler = signal_reset_prompt;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART;
	sigaction(SIGINT, &act, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	set_signals_noninteractive(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

//void    set_signals_heredoc()
