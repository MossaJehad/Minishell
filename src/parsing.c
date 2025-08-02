/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 19:09:28 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/02 12:51:18 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Enhanced parsing function - replace your parse_arguments function

char	**parse_arguments(const char *input, int *arg_count)
{
	t_parse_state	s;
	char			**argv;
	char			buffer[BUFFER_SIZE];
	char			next;

	argv = ft_calloc(MAX_ARGS, sizeof(char *));
	nully(&s);
	while (input[s.i])
	{
		// Handle quotes
		if (input[s.i] == '\'' && !s.in_double_quote)
		{
			s.in_single_quote = !s.in_single_quote;
			buffer[s.j++] = input[s.i++];
			continue ;
		}
		if (input[s.i] == '"' && !s.in_single_quote)
		{
			s.in_double_quote = !s.in_double_quote;
			buffer[s.j++] = input[s.i++];
			continue ;
		}
		// Handle whitespace (only when not in quotes)
		if (!s.in_single_quote && !s.in_double_quote && (input[s.i] == ' '
				|| input[s.i] == '\t'))
		{
			if (s.j > 0)
			{
				buffer[s.j] = '\0';
				argv[s.k++] = ft_strdup(buffer);
				s.j = 0;
			}
			s.i++;
			continue ;
		}
		// NEW: Handle operators when not in quotes
		if (!s.in_single_quote && !s.in_double_quote)
		{
			// Check for << (must come before single <)
			if (input[s.i] == '<' && input[s.i + 1] == '<')
			{
				if (s.j > 0)
				{
					buffer[s.j] = '\0';
					argv[s.k++] = ft_strdup(buffer);
					s.j = 0;
				}
				argv[s.k++] = ft_strdup("<<");
				s.i += 2;
				continue ;
			}
			// Check for >> (must come before single >)
			if (input[s.i] == '>' && input[s.i + 1] == '>')
			{
				if (s.j > 0)
				{
					buffer[s.j] = '\0';
					argv[s.k++] = ft_strdup(buffer);
					s.j = 0;
				}
				argv[s.k++] = ft_strdup(">>");
				s.i += 2;
				continue ;
			}
			// Check for single operators
			if (input[s.i] == '|' || input[s.i] == '<' || input[s.i] == '>')
			{
				if (s.j > 0)
				{
					buffer[s.j] = '\0';
					argv[s.k++] = ft_strdup(buffer);
					s.j = 0;
				}
				buffer[0] = input[s.i];
				buffer[1] = '\0';
				argv[s.k++] = ft_strdup(buffer);
				s.i++;
				continue ;
			}
		}
		// Handle escape sequences
		if ((s.in_single_quote || s.in_double_quote) && (input[s.i] == '\\'
				&& input[s.i + 1] == 'n'))
		{
			buffer[s.j++] = input[s.i++];
			continue ;
		}
		if (input[s.i] == '\\' && input[s.i + 1])
		{
			next = input[s.i + 1];
			if (!s.in_single_quote && !s.in_double_quote)
			{
				buffer[s.j++] = next;
				s.i += 2;
				continue ;
			}
			else
			{
				buffer[s.j++] = input[s.i++];
				buffer[s.j++] = input[s.i++];
				continue ;
			}
		}
		// Regular character
		buffer[s.j++] = input[s.i++];
	}
	// Handle final token
	if (s.j > 0)
	{
		buffer[s.j] = '\0';
		argv[s.k++] = ft_strdup(buffer);
	}
	argv[s.k] = NULL;
	*arg_count = s.k;
	return (argv);
}
