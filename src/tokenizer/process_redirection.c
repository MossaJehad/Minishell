/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:26:46 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/29 17:02:08 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_redirection_type(char *op)
{
	if (ft_strcmp(op, ">") == 0)
		return ("output");
	else if (ft_strcmp(op, "<") == 0)
		return ("input");
	else if (ft_strcmp(op, ">>") == 0)
		return ("append output");
	else if (ft_strcmp(op, "<<") == 0)
		return ("here-document");
	return ("unknown");
}

int	process_redirection(char **array, int *i, t_token **token, char *op)
{
	char	*type;

	if (ft_strcmp(array[*i], op) == 0)
	{
		type = get_redirection_type(op);
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
	if (process_redirection(array, i, token, ">>"))
		return (1);
	if (process_redirection(array, i, token, "<<"))
		return (1);
	return (0);
}
