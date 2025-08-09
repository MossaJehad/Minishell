/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:04 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 12:34:52 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
volatile sig_atomic_t	g_signal;

void	null_parse_state(t_parse_state *s)
{
	s->i = 0;
	s->j = 0;
	s->k = 0;
	s->in_single_quote = 0;
	s->in_double_quote = 0;
}

void	shell_loop(int arg_count, char ***envp)
{
	char	*input;
	char	**args;
	t_token	*token;

	while (1)
	{
		g_signal = 0;
		token = NULL;
		input = get_input();
		if (input == NULL)
			break ;
		if (*input == '\0')
		{
			free(input);
			continue ;
		}
		args = parse_arguments(input, &arg_count);
		args = expand(args, *envp);
		if (!check_syntax_error(args))
			tokenize(args, &token);
		handle_command(token, envp);
		ft_free((void **) &args);
		free_tokens(token);
		free(input);
	}
}

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
	cleanup_and_exit(get_shell_status(), env);
	return (0);
}
