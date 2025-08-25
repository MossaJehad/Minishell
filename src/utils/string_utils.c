/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:28:48 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/25 18:43:30 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * Checks whether the given input string contains unclosed quotes.
 * It iterates through the string and tracks whether the parser is
 * currently inside single quotes (') or double quotes (").
 * A quote is considered unclosed if one type is opened but not closed
 * before the end of the string.
 *
 * @param input the input string to check
 *
 * @return 1 if there are unclosed quotes, 0 otherwise
 */
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

int	is_blank(const char *s)
{
	if (!s)
		return (1);
	while (*s)
	{
		if (!ft_isspace((unsigned char)*s))
			return (0);
		s++;
	}
	return (1);
}
