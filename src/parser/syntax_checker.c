/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:31:06 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 17:40:23 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int handle_double_less(const char *input, t_parse_state *s, char **argv, char *buffer)
{
	if (!s->in_single_quote && !s->in_double_quote &&
		input[s->i] == '<' && input[s->i + 1] == '<')
	{
		if (s->j > 0)
		{
			buffer[s->j] = '\0';
			argv[s->k++] = ft_strdup(buffer);
			s->j = 0;
		}
		argv[s->k++] = ft_strdup("<<");
		s->i += 2;
		return (1);
	}
	return (0);
}

int handle_double_greater(const char *input, t_parse_state *s, char **argv, char *buffer)
{
	if (!s->in_single_quote && !s->in_double_quote &&
		input[s->i] == '>' && input[s->i + 1] == '>')
	{
		if (s->j > 0)
		{
			buffer[s->j] = '\0';
			argv[s->k++] = ft_strdup(buffer);
			s->j = 0;
		}
		argv[s->k++] = ft_strdup(">>");
		s->i += 2;
		return (1);
	}
	return (0);
}

int handle_double_char_operators(const char *input, t_parse_state *s, char **argv, char *buffer)
{
	if (handle_double_less(input, s, argv, buffer))
		return (1);
	if (handle_double_greater(input, s, argv, buffer))
		return (1);
	return (0);
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
			return (1);
		}
	}
	return (0);
}

int handle_escape_sequences(const char *input, t_parse_state *s, char *buffer)
{
	if ((s->in_single_quote || s->in_double_quote) && (input[s->i] == '\\' && input[s->i + 1] == 'n'))
	{
		buffer[s->j++] = input[s->i++];
		return (1);
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
		return (1);
	}
	return (0);
}
