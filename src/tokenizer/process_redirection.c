/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:26:46 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/23 20:35:20 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	process_redirection(char **array, int *i, t_token **token,
	char *op, char *type)
{
	if (ft_strcmp(array[*i], op) == 0)
	{
		create_token(token, op, type);
		(*i)++;
		if (!array[*i])
		{
			printf("syntax error: unexpected end after `%s`\n", op);
			return (-1);
		}
		create_token(token, array[*i], "word");
		(*i)++;
		return (1);
	}
	return (0);
}

int	tokenize_append_and_heredoc(char **array, int *i, t_token **token)
{
	if (process_redirection(array, i, token, ">>", "append output"))
		return (1);
	if (process_redirection(array, i, token, "<<", "here-document"))
		return (1);
	return (0);
}
