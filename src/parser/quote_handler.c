/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:30:02 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 17:39:55 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
		return (1);
	}
	return (0);
}

int handle_quotes(const char *input, t_parse_state *s, char *buffer)
{
	if (input[s->i] == '\'' && !s->in_double_quote)
	{
		s->in_single_quote = !s->in_single_quote;
		buffer[s->j++] = input[s->i++];
		return (1);
	}
	if (input[s->i] == '"' && !s->in_single_quote)
	{
		s->in_double_quote = !s->in_double_quote;
		buffer[s->j++] = input[s->i++];
		return (1);
	}
	return (0);
}
