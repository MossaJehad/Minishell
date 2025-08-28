/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:24:17 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/28 21:10:12 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_valid_after_pipe(char *op)
{
	if (ft_strcmp(op, "|") == 0)
		return (0);
	return (1);
}

int	check_operator_validity(char *current_op, char *next_op)
{
	if (ft_strcmp(current_op, "|") == 0)
		return (is_valid_after_pipe(next_op));
	if (is_operator(current_op) && is_operator(next_op))
		return (0);
	return (1);
}

int	check_operator_sequence(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		if (is_operator(array[i]))
		{
			if (!array[i + 1])
			{
				printf("syntax error near unexpected token: newline\n");
				return (1);
			}
			if (is_operator(array[i + 1]))
			{
				if (!check_operator_validity(array[i], array[i + 1]))
				{
					printf("syntax error near unexpected token: %s\n",
						array[i + 1]);
					return (1);
				}
			}
		}
		i++;
	}
	return (0);
}

int	check_leading_pipe(char **array)
{
	if (array[0] && ft_strcmp(array[0], "|") == 0)
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		return (1);
	}
	return (0);
}

int	check_syntax_error(char **array)
{
	if (!array || !array[0])
		return (0);
	if (check_leading_pipe(array))
		return (1);
	return (check_operator_sequence(array));
}
