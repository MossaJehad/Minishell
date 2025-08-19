/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:40:06 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/19 01:12:41 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	t_token	*peek;

	while (token)
	{
		if (token->type == WORD || token->type == QUOTED_STRING || token->type == COMMAND)
		{
			printf("%s", token->value);
			peek = token->next;
			while (peek && !(peek->type == WORD || peek->type == QUOTED_STRING || peek->type == COMMAND))
				peek = peek->next;
			if (peek && peek->value && *peek->value)
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
	token = skip_n_flags(token, &newline);
	print_echo_args(token, newline);
}