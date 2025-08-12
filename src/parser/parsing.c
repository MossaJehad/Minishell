/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 19:09:28 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/12 11:05:23 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	run_parsing_loop(const char *input, t_parse_state *s, char **argv,
		char *buffer)
{
	while (input[s->i])
	{
		if (handle_quotes(input, s, buffer))
			continue ;
		if (handle_whitespace(input, s, argv, buffer))
			continue ;
		if (handle_double_char_operators(input, s, argv, buffer))
			continue ;
		if (handle_single_char_operators(input, s, argv, buffer))
			continue ;
		if (handle_escape_sequences(input, s, buffer))
			continue ;
		buffer[s->j++] = input[s->i++];
	}
}

char	**parse_arguments(const char *input, int *arg_count)
{
	t_parse_state	s;
	char			**argv;
	char			buffer[BUFFER_SIZE];
	int				cleanup_i;

	if (!input || !arg_count)
		return (NULL);
	argv = ft_calloc(MAX_ARGS, sizeof(char *));
	if (!argv)
		return (NULL);
	null_parse_state(&s);
	run_parsing_loop(input, &s, argv, buffer);
	if (s.j > 0)
	{
		buffer[s.j] = '\0';
		argv[s.k] = ft_strdup(buffer);
		if (!argv[s.k])
		{
			cleanup_i = 0;
			while (cleanup_i < s.k)
			{
				if (argv[cleanup_i])
					free(argv[cleanup_i]);
				cleanup_i++;
			}
			free(argv);
			return (NULL);
		}
		s.k++;
	}
	argv[s.k] = NULL;
	*arg_count = s.k;
	return (argv);
}
