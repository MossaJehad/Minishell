/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 20:29:22 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/28 16:18:16 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

volatile sig_atomic_t	g_signal;

int	main(int ac, char *av[], char **envp)
{
	char	**env;
	size_t	env_count;

	(void)av;
	setup_signal_handlers();
	env_count = get_length(envp);
	env = ft_calloc((env_count + 1), sizeof(char *));
	if (!env)
		return (1);
	copy_env(env, envp);
	init_shell(env);
	shell_loop(ac, &env);
	if (env)
		ft_free_arr(env);
	rl_clear_history();
	return (get_shell_status());
}

void	null_parse_state(t_parse_state *s)
{
	s->cursor = 0;
	s->buffer_pos = 0;
	s->arg_count = 0;
	s->in_single_quote = 0;
	s->in_double_quote = 0;
}

void	setup_signal_handlers(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
