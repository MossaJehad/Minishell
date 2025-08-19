/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:30:02 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/17 20:21:31 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_whitespace(const char *input, t_parse_state *s, char **argv,
		char *buffer)
{
	char	*token;

	if (!s->in_single_quote && !s->in_double_quote && (input[s->cursor] == ' '
			|| input[s->cursor] == '\t'))
	{
		if (s->buffer_pos > 0)
		{
			buffer[s->buffer_pos] = '\0';
			token = ft_strdup(buffer);
			if (!token)
				return (-1);
			argv[s->arg_count++] = token;
			s->buffer_pos = 0;
		}
		s->cursor++;
		return (1);
	}
	return (0);
}

int	handle_quotes(const char *input, t_parse_state *s, char *buffer)
{
	if (input[s->cursor] == '\'' && !s->in_double_quote)
	{
		s->in_single_quote = !s->in_single_quote;
		buffer[s->buffer_pos++] = input[s->cursor++];
		return (1);
	}
	if (input[s->cursor] == '"' && !s->in_single_quote)
	{
		s->in_double_quote = !s->in_double_quote;
		buffer[s->buffer_pos++] = input[s->cursor++];
		return (1);
	}
	return (0);
}
