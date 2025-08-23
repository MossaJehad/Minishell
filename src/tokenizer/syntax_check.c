/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:24:17 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/23 22:15:45 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_operator_sequence(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		if (is_operator(array[i]))
		{
			if (!array[i + 1] || is_operator(array[i + 1]))
			{
				if (array[i + 1])
					printf("minishell: syntax error near unexpected token: \
						%s\n", array[i + 1]);
				else
					printf("minishell: syntax error near unexpected token: \
						newline\n");
				return (1);
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
