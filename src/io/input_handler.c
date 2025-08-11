/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:32:55 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/10 16:39:11 by mhasoneh         ###   ########.fr       */
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
	if (!tok->type)
	{
		printf("setup_redirection: token type is null\n");
		return (-1);
	}
	if (ft_strcmp(tok->type, "here-document") == 0 && !tok->value)
	{
		printf("heredoc: missing delimiter\n");
		return (-1);
	}
	return (0);
}

char	*append_until_quotes_closed(char *line)
{
	char	*new_part;
	char	*combined;
	char	*temp;

	while (has_unclosed_quotes(line))
	{
		new_part = readline("> ");
		if (g_signal)
		{
			g_signal = 0;
			if (new_part)
				free(new_part);
			free(line);
			return (NULL);
		}
		if (!new_part)
		{
			free(line);
			return (NULL);
		}
		combined = ft_strjoin(line, "\n");
		if (!combined)
		{
			free(line);
			free(new_part);
			return (NULL);
		}
		temp = ft_strjoin(combined, new_part);
		free(line);
		free(combined);
		free(new_part);
		if (!temp)
			return (NULL);
		line = temp;
	}
	return (line);
}
