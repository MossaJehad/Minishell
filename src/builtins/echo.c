/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:40:06 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 05:29:38 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_echo(t_shell *shell, char **args)
{
	int	i;
	int	newline;
	int	n_flag_len;

	(void)shell;
	
	newline = 1;
	i = 1;

	/* Check for -n flag */
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		n_flag_len = 2;
		while (args[i][n_flag_len] == 'n')
			n_flag_len++;
		
		if (args[i][n_flag_len] == '\0')
		{
			newline = 0;
			i++;
		}
		else
			break;
	}

	/* Print arguments */
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}

	if (newline)
		printf("\n");

	return (EXIT_SUCCESS);
}

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
		if (ft_strcmp(token->value, "word") == 0 || ft_strcmp(token->value,
				"command") == 0)
		{
			printf("%s", token->value);
			peek = token->next;
			while (peek && !(ft_strcmp(peek->value, "word") == 0
					|| ft_strcmp(peek->value, "command") == 0))
				peek = peek->next;
			if (peek)
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
