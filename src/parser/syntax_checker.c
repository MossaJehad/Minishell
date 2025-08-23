/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:31:06 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/17 20:22:56 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_double_less(const char *input, t_parse_state *s, char **argv,
		char *buffer)
{
	char	*token;

	if (!s->in_single_quote && !s->in_double_quote && input[s->cursor] == '<'
		&& input[s->cursor + 1] == '<')
	{
		if (s->buffer_pos > 0)
		{
			buffer[s->buffer_pos] = '\0';
			argv[s->arg_count++] = ft_strdup(buffer);
			if (!argv[s->arg_count - 1])
				return (-1);
			s->buffer_pos = 0;
		}
		token = ft_strdup("<<");
		if (!token)
			return (-1);
		argv[s->arg_count++] = token;
		s->cursor += 2;
		return (1);
	}
	return (0);
}

int	handle_double_greater(const char *input, t_parse_state *s, char **argv,
		char *buffer)
{
	char	*token;

	if (!s->in_single_quote && !s->in_double_quote && input[s->cursor] == '>'
		&& input[s->cursor + 1] == '>')
	{
		if (s->buffer_pos > 0)
		{
			buffer[s->buffer_pos] = '\0';
			argv[s->arg_count++] = ft_strdup(buffer);
			if (!argv[s->arg_count - 1])
				return (-1);
			s->buffer_pos = 0;
		}
		token = ft_strdup(">>");
		if (!token)
			return (-1);
		argv[s->arg_count++] = token;
		s->cursor += 2;
		return (1);
	}
	return (0);
}

int	handle_double_char_operators(const char *input, t_parse_state *s,
		char **argv, char *buffer)
{
	if (handle_double_less(input, s, argv, buffer))
		return (1);
	if (handle_double_greater(input, s, argv, buffer))
		return (1);
	return (0);
}

int	handle_single_char_operators(const char *input, t_parse_state *s,
		char **argv, char *buffer)
{
	char	operator_str[2];
	char	*token;

	if (!s->in_single_quote && !s->in_double_quote)
	{
		if (input[s->cursor] == '|' || input[s->cursor] == '<'
			|| input[s->cursor] == '>')
		{
			if (s->buffer_pos > 0)
			{
				buffer[s->buffer_pos] = '\0';
				argv[s->arg_count++] = ft_strdup(buffer);
				if (!argv[s->arg_count - 1])
					return (-1);
				s->buffer_pos = 0;
			}
			operator_str[0] = input[s->cursor];
			operator_str[1] = '\0';
			token = ft_strdup(operator_str);
			if (!token)
				return (-1);
			argv[s->arg_count++] = token;
			s->cursor++;
			return (1);
		}
	}
	return (0);
}

int	handle_escape_sequences(const char *input, t_parse_state *s, char *buffer)
{
	char	next;

	if ((s->in_single_quote || s->in_double_quote) && (input[s->cursor] == '\\'
			&& input[s->cursor + 1] == 'n'))
	{
		buffer[s->buffer_pos++] = input[s->cursor++];
		return (1);
	}
	if (input[s->cursor] == '\\' && input[s->cursor + 1])
	{
		next = input[s->cursor + 1];
		if (!s->in_single_quote && !s->in_double_quote)
		{
			buffer[s->buffer_pos++] = next;
			s->cursor += 2;
		}
		else
		{
			buffer[s->buffer_pos++] = input[s->cursor++];
			buffer[s->buffer_pos++] = input[s->cursor++];
		}
		return (1);
	}
	return (0);
}
