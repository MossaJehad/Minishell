/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 19:09:28 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/07/22 18:09:15 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Add this function to properly free the argv array
void ft_free(char **array)
{
    int i = 0;
    if (!array)
        return;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

char **parse_arguments(const char *input, int *arg_count)
{
	int				i;
	char			**argv;
	char			buffer[BUFFER_SIZE];
	char			next;
	t_parse_state	s;
	
	argv = malloc(sizeof(char *) * MAX_ARGS);
	if (!argv)
		return (NULL);
	i = 0;
	while (i < MAX_ARGS)
		argv[i++] = NULL;
	nully(&s);
	while (input[s.i])
	{
		if (input[s.i] == '\'' && !s.in_double_quote)
		{
			s.in_single_quote = !s.in_single_quote;
			buffer[s.j++] = input[s.i++];
			continue;
		}
		if (input[s.i] == '"' && !s.in_single_quote)
		{
			s.in_double_quote = !s.in_double_quote;
			buffer[s.j++] = input[s.i++];
			continue;
		}
		if (!s.in_single_quote && !s.in_double_quote && 
			(input[s.i] == ' ' || input[s.i] == '\t'))
		{
			if (s.j > 0)
			{
				buffer[s.j] = '\0';
				argv[s.k] = ft_strdup(buffer);
				if (!argv[s.k])
				{
					while (--s.k >= 0)
						free(argv[s.k]);
					free(argv);
					return (NULL);
				}
				s.k++;
				s.j = 0;
			}
			s.i++;
			continue;
		}
		if ((s.in_single_quote || s.in_double_quote) && 
			(input[s.i] == '\\' && input[s.i + 1] == 'n'))
		{
			buffer[s.j++] = input[s.i++];
			continue;
		}
		if (input[s.i] == '\\' && input[s.i + 1])
		{
			next = input[s.i + 1];
			if (!s.in_single_quote && !s.in_double_quote)
			{
				buffer[s.j++] = next;
				s.i += 2;
				continue;
			}
			else
			{
				buffer[s.j++] = input[s.i++];
				buffer[s.j++] = input[s.i++];
				continue;
			}
		}
		buffer[s.j++] = input[s.i++];
	}
	if (s.j > 0)
	{
		buffer[s.j] = '\0';
		argv[s.k] = ft_strdup(buffer);
		if (!argv[s.k])
		{
			// Clean up on malloc failure
			while (--s.k >= 0)
				free(argv[s.k]);
			free(argv);
			return (NULL);
		}
		s.k++;
	}
	argv[s.k] = NULL;
	*arg_count = s.k;
	return (argv);
}
