/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:14:31 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/30 13:39:25 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	run_parsing_loop(const char *input, t_parse_state *s, char **argv,
		char *buffer)
{
	while (input[s->cursor])
	{
		if (handle_quotes(input, s, buffer))
		{
			if (!s->in_single_quote && !s->in_double_quote)
			{
				if (s->buffer_pos == 2
					&& ((buffer[0] == '\'' && buffer[1] == '\'')
						|| (buffer[0] == '"' && buffer[1] == '"')))
				{
					buffer[s->buffer_pos] = '\0';
					argv[s->arg_count++] = ft_strdup(buffer);
					s->buffer_pos = 0;
				}
			}
			continue ;
		}
		if (handle_whitespace(input, s, argv, buffer))
			continue ;
		if (handle_double_char_operators(input, s, argv, buffer))
			continue ;
		if (handle_single_char_operators(input, s, argv, buffer))
			continue ;
		if (handle_escape_sequences(input, s, buffer))
			continue ;
		buffer[s->buffer_pos++] = input[s->cursor++];
	}
}

char	**allocate_argv(void)
{
	char	**argv;

	argv = ft_calloc(MAX_ARGS, sizeof(char *));
	return (argv);
}

int	handle_final_buffer(char **argv, t_parse_state *s, char *buffer)
{
	int	i;

	if (s->buffer_pos > 0)
	{
		buffer[s->buffer_pos] = '\0';
		argv[s->arg_count] = ft_strdup(buffer);
		if (!argv[s->arg_count])
		{
			i = 0;
			while (i < s->arg_count)
			{
				free(argv[i]);
				i++;
			}
			free(argv);
			return (-1);
		}
		s->arg_count++;
	}
	return (0);
}

char	**parse_arguments(const char *input, int *arg_count)
{
	t_parse_state	s;
	char			**argv;
	char			buffer[BUFFER_SIZE];

	if (!input || !arg_count)
		return (NULL);
	argv = allocate_argv();
	if (!argv)
		return (NULL);
	null_parse_state(&s);
	run_parsing_loop(input, &s, argv, buffer);
	if (handle_final_buffer(argv, &s, buffer) == -1)
		return (NULL);
	argv[s.arg_count] = NULL;
	*arg_count = s.arg_count;
	return (argv);
}
