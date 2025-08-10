/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:04 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/10 14:39:10 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

volatile sig_atomic_t	g_signal;

int	main(int argc, char **argv, char **envp)
{
	char	**env;
	int		i;

	(void)argv;
	setup_signal_handlers();
	i = 0;
	while (envp[i])
		i++;
	env = malloc(sizeof(char *) * (i + 1));
	if (!env)
		return (1);
	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		i++;
	}
	env[i] = NULL;
	init_shell(env);
	shell_loop(argc, &env);
	rl_clear_history();
	ft_free_arr((void *)&env);
	return (get_shell_status());
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

void	null_shell(t_shell *shl)
{
	shl->oldpwd = NULL;
	shl->shlvl = NULL;
	shl->pwd = NULL;
}
