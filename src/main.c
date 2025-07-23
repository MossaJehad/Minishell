/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:04 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/07/23 14:45:02 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>

char	*ft_strndup(const char *s1, size_t n)
{
	char	*copy;
	size_t	len;

	len = ft_strlen(s1);
	if (n < len)
		len = n;
	copy = malloc(len + 1);
	if (!copy)
		return (NULL);
	ft_memcpy(copy, s1, len);
	copy[len] = '\0';
	return (copy);
}

int	has_unclosed_quotes(const char *input)
{
	int	in_single;
	int	in_double;

	in_double = 0;
	in_single = 0;
	while (*input)
	{
		if (*input == '\'' && !in_double)
			in_single = !in_single;
		else if (*input == '"' && !in_single)
			in_double = !in_double;
		input++;
	}
	return (in_single || in_double);
}

void	nully(t_parse_state *s)
{
	s->i = 0;
	s->j = 0;
	s->k = 0;
	s->in_single_quote = 0;
	s->in_double_quote = 0;
}

void shell_loop(int arg_count, char ***envp)
{
	char	*input;
	char	**args;
	t_token	*token;

	while (1)
	{
		token = NULL;
		input = get_input();
		if (input == NULL)
			break;
		if (*input == '\0')
		{
			free(input);
			continue;
		}
		args = parse_arguments(input, &arg_count);
		if (!args)
		{
			free(input);
			continue;
		}
		if (check_syntax_error(args))
		{
			ft_free(args);
			free(input);
			continue;
		}
		args = expand(args, *envp);
		tokenize(args, &token);
		handle_command(input, args, arg_count, token, envp);
		//free(envp);
		ft_free(args);
		free_tokens(token);
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	**env;

	(void)argv;
	init_shell(envp);
	env = copy_env(envp);
	if (!env)
		return (1);
	shell_loop(argc, &env);
	free_env(env);
	rl_clear_history();
	return (0);
}
