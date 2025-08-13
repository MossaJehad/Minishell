/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:04 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 05:04:55 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

volatile sig_atomic_t	g_signal = 0;

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argv;
	(void)argc;
	ft_memset(&shell, 0, sizeof(t_shell));
	setup_signals();
	init_shell(&shell, envp);
	shell_loop(&shell);
	cleanup_and_exit(&shell, shell.exit_status);
	return (shell.exit_status);
}

/*
**	Those used to give a value for the struct
*/
void	null_parse_state(t_parse_state *s)
{
	s->i = 0;
	s->j = 0;
	s->k = 0;
	s->in_single_quote = 0;
	s->in_double_quote = 0;
}
