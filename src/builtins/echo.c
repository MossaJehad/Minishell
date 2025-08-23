/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:40:06 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/23 15:15:26 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>

int	count_n_flags(t_token *token)
{
	int		n_count;
	char	*flag;
	int		i;

	n_count = 0;
	while (token && ft_strncmp(token->value, "-", 1) == 0)
	{
		flag = token->value;
		if (ft_strlen(flag) < 2)
			break ;
		i = 1;
		while (flag[i])
		{
			if (flag[i] != 'n')
				return (n_count);
			i++;
		}
		n_count++;
		token = token->next;
	}
	return (n_count);
}

t_token	*skip_n_flags(t_token *token, int *newline)
{
	int	n_flags;

	n_flags = count_n_flags(token);
	if (n_flags > 0)
	{
		*newline = 0;
		while (n_flags-- > 0 && token)
			token = token->next;
	}
	return (token);
}

void	print_echo_args(t_token *token, int newline)
{
	while (token)
	{
		if ((token->type == WORD || token->type == QUOTED_STRING
				|| token->type == COMMAND) && token->value)
		{
			printf("%s", token->value);
			if (token->next && (token->next->type == WORD
					|| token->next->type == QUOTED_STRING
					|| token->next->type == COMMAND))
				printf(" ");
		}
		token = token->next;
	}
	if (newline)
		printf("\n");
}

void	handle_echo_command(t_token *token)
{
	int	newline;

	newline = 1;
	token = token->next;
	printf("%s\n", token->value);
	token = skip_n_flags(token, &newline);
	print_echo_args(token, newline);
}
