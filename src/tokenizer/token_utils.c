/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:38:00 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/17 20:24:16 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_command(char *word)
{
	if (ft_strcmp(word, "echo") == 0 || ft_strcmp(word, "cd") == 0
		|| ft_strcmp(word, "pwd") == 0 || ft_strcmp(word, "export") == 0
		|| ft_strcmp(word, "unset") == 0 || ft_strcmp(word, "env") == 0
		|| ft_strcmp(word, "exit") == 0)
		return (1);
	else if (ft_strcmp(word, "type") == 0 || ft_strcmp(word, "cat") == 0
		|| ft_strcmp(word, "ls") == 0 || ft_strcmp(word, "clear") == 0)
		return (1);
	return (0);
}

void	free_tokens(t_token *token)
{
	t_token	*temp;

	while (token)
	{
		temp = token;
		token = token->next;
		free(temp->value);
		free(temp);
	}
}
