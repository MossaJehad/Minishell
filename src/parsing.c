/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 19:09:28 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 12:43:56 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


int handle_quotes(const char *input, t_parse_state *s, char *buffer)
{
	if (input[s->i] == '\'' && !s->in_double_quote)
	{
		s->in_single_quote = !s->in_single_quote;
		buffer[s->j++] = input[s->i++];
		return 1;
	}
	if (input[s->i] == '"' && !s->in_single_quote)
	{
		s->in_double_quote = !s->in_double_quote;
		buffer[s->j++] = input[s->i++];
		return 1;
	}
	return 0;
}

int handle_whitespace(const char *input, t_parse_state *s, char **argv, char *buffer)
{
	if (!s->in_single_quote && !s->in_double_quote && (input[s->i] == ' ' || input[s->i] == '\t'))
	{
		if (s->j > 0)
		{
			buffer[s->j] = '\0';
			argv[s->k++] = ft_strdup(buffer);
			s->j = 0;
		}
		s->i++;
		return 1;
	}
	return 0;
}

int handle_double_char_operators(const char *input, t_parse_state *s, char **argv, char *buffer)
{
	if (!s->in_single_quote && !s->in_double_quote)
	{
		if (input[s->i] == '<' && input[s->i + 1] == '<')
		{
			if (s->j > 0)
			{
				buffer[s->j] = '\0';
				argv[s->k++] = ft_strdup(buffer);
				s->j = 0;
			}
			argv[s->k++] = ft_strdup("<<");
			s->i += 2;
			return 1;
		}
		if (input[s->i] == '>' && input[s->i + 1] == '>')
		{
			if (s->j > 0)
			{
				buffer[s->j] = '\0';
				argv[s->k++] = ft_strdup(buffer);
				s->j = 0;
			}
			argv[s->k++] = ft_strdup(">>");
			s->i += 2;
			return 1;
		}
	}
	return 0;
}

int handle_single_char_operators(const char *input, t_parse_state *s, char **argv, char *buffer)
{
	if (!s->in_single_quote && !s->in_double_quote)
	{
		if (input[s->i] == '|' || input[s->i] == '<' || input[s->i] == '>')
		{
			if (s->j > 0)
			{
				buffer[s->j] = '\0';
				argv[s->k++] = ft_strdup(buffer);
				s->j = 0;
			}
			buffer[0] = input[s->i];
			buffer[1] = '\0';
			argv[s->k++] = ft_strdup(buffer);
			s->i++;
			return 1;
		}
	}
	return 0;
}

int handle_escape_sequences(const char *input, t_parse_state *s, char *buffer)
{
	if ((s->in_single_quote || s->in_double_quote) && (input[s->i] == '\\' && input[s->i + 1] == 'n'))
	{
		buffer[s->j++] = input[s->i++];
		return 1;
	}
	if (input[s->i] == '\\' && input[s->i + 1])
	{
		char next = input[s->i + 1];
		if (!s->in_single_quote && !s->in_double_quote)
		{
			buffer[s->j++] = next;
			s->i += 2;
		}
		else
		{
			buffer[s->j++] = input[s->i++];
			buffer[s->j++] = input[s->i++];
		}
		return 1;
	}
	return 0;
}

void run_parsing_loop(const char *input, t_parse_state *s, char **argv, char *buffer)
{
	while (input[s->i])
	{
		if (handle_quotes(input, s, buffer))
			continue;
		if (handle_whitespace(input, s, argv, buffer))
			continue;
		if (handle_double_char_operators(input, s, argv, buffer))
			continue;
		if (handle_single_char_operators(input, s, argv, buffer))
			continue;
		if (handle_escape_sequences(input, s, buffer))
			continue;
		buffer[s->j++] = input[s->i++];
	}
}

char	**parse_arguments(const char *input, int *arg_count)
{
	t_parse_state	s;
	char			**argv;
	char			buffer[BUFFER_SIZE];

	argv = ft_calloc(MAX_ARGS, sizeof(char *));
	null_parse_state(&s);
	run_parsing_loop(input, &s, argv, buffer);
	if (s.j > 0)
	{
		buffer[s.j] = '\0';
		argv[s.k++] = ft_strdup(buffer);
	}
	argv[s.k] = NULL;
	*arg_count = s.k;
	return argv;
}
