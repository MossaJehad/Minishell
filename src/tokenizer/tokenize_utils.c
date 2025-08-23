/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:50:45 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/23 20:51:07 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	tokenize_pipe(char **array, int *i, t_token **token)
{
	if (ft_strcmp(array[*i], "|") == 0)
	{
		create_token(token, "|", "pipe");
		(*i)++;
		return (1);
	}
	return (0);
}

int	tokenize_pipe_and_redirects(char **array, int *i, t_token **token)
{
	if (tokenize_pipe(array, i, token))
		return (1);
	return (tokenize_redirects(array, i, token));
}

void	validate_tokenize_input(char **array, t_token **token,
			int *should_continue)
{
	if (!array || !token)
	{
		*should_continue = 0;
		return ;
	}
	*should_continue = 1;
}

void	create_command_or_word_token(char **array, int i, t_token **token)
{
	if (check_command(array[i]))
		create_token(token, array[i], "command");
	else
		create_token(token, array[i], "word");
}
