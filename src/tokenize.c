/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:30 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/07/21 19:15:57 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_token(t_token **token, char *value, char *type)
{
	t_token	*new;
	t_token	*temp;

	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	new->value = ft_strdup(value);
	new->type = ft_strdup(type);
	new->next = NULL;
	if (!*token)
		*token = new;
	else
	{
		temp = *token;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
}

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

int	check_syntax_error(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		if (ft_strcmp(array[i], "|") == 0 || ft_strcmp(array[i], "<") == 0
			|| ft_strcmp(array[i], ">") == 0 || ft_strcmp(array[i], ">>") == 0
			|| ft_strcmp(array[i], "<<") == 0)
		{
			if (!array[++i])
			{
				printf("syntax error\n");
				return (1);
			}
		}
		i++;
	}
	return (0);
}

void	tokenize(char **array, t_token **token)
{
	int	i;

	i = 0;
	while (array[i])
	{
		if (check_command(array[i]))
			create_token(token, array[i], "command");
		else if (ft_strcmp(array[i], "|") == 0)
			create_token(token, "|", "pipe");
		else if (ft_strcmp(array[i], "<") == 0)
			create_token(token, array[++i], "redirect input");
		else if (ft_strcmp(array[i], ">") == 0)
			create_token(token, array[++i], "redirect output");
		else if (ft_strcmp(array[i], ">>") == 0)
			create_token(token, array[++i], "append output");
		else if (ft_strcmp(array[i], "<<") == 0)
			create_token(token, array[++i], "here-document");
		else
			create_token(token, array[i], "word");
		i++;
	}
}

void	free_tokens(t_token *token)
{
	t_token	*temp;

	while (token)
	{
		temp = token;
		token = token->next;
		free(temp->value);
		free(temp->type);
		free(temp);
	}
}
