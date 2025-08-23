/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 13:11:53 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/23 14:31:46 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	validate_token(t_token *tok)
{
	if (!tok)
	{
		printf("setup_redirection: null token\n");
		return (-1);
	}
	if (tok->type == HEREDOC && !tok->value)
	{
		printf("heredoc: missing delimiter\n");
		return (-1);
	}
	return (0);
}

char	*append_until_quotes_closed(char *line)
{
	if (has_unclosed_quotes(line))
	{
		printf("Syntax error \n");
		free(line);
		return (NULL);
	}
	return (line);
}
