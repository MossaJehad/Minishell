/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:30 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/30 15:25:26 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	find_type(t_token *new, char *type)
{
	if (ft_strcmp(type, "command") == 0)
		new->type = COMMAND;
	else if (ft_strcmp(type, "word") == 0)
		new->type = WORD;
	else if (ft_strcmp(type, "pipe") == 0)
		new->type = PIPE;
	else if (ft_strcmp(type, "redirect input") == 0)
		new->type = REDIRECT;
	else if (ft_strcmp(type, "redirect output") == 0)
		new->type = REDIRECT_OUT;
	else if (ft_strcmp(type, "append output") == 0)
		new->type = APPEND;
	else if (ft_strcmp(type, "here-document") == 0)
		new->type = HEREDOC;
	else
		new->type = WORD;
}

void	create_token(t_token **token, char *value, char *type)
{
	t_token	*new;
	t_token	*temp;

	if (!value || !type)
		return ;
	new = ft_calloc(1, sizeof(t_token));
	new->value = ft_strdup(value);
	new->next = NULL;
	find_type(new, type);
	if (!new->value)
	{
		free(new);
		return ;
	}
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

int	tokenize_redirects(char **array, int *i, t_token **token)
{
	if (ft_strcmp(array[*i], "<") == 0)
	{
		create_token(token, "<", "redirect input");
		(*i)++;
		if (!array[*i])
			return (perror("syntax error: unexpected end after `<`\n"), -1);
		create_token(token, array[*i], "word");
		(*i)++;
		return (1);
	}
	else if (ft_strcmp(array[*i], ">") == 0)
	{
		create_token(token, ">", "redirect output");
		(*i)++;
		if (!array[*i])
			return (perror("syntax error: unexpected end after `>`\n"), -1);
		create_token(token, array[*i], "word");
		(*i)++;
		return (1);
	}
	return (0);
}

int	handle_token_dispatch(char **array, int *i, t_token **token, int *is_first)
{
	int	result;

	result = tokenize_append_and_heredoc(array, i, token);
	if (result == -1)
	{
		free_tokens(*token);
		*token = NULL;
		return (-1);
	}
	if (result == 1)
	{
		*is_first = 0;
		return (1);
	}
	result = tokenize_pipe_and_redirects(array, i, token);
	if (result == 1)
	{
		*is_first = (ft_strcmp(array[*i - 1], "|") == 0);
		return (1);
	}
	create_command_or_word_token(array, *i, token);
	*is_first = 0;
	return (0);
}

void	process_tokens(char **array, t_token **token)
{
	int	i;
	int	is_first;
	int	result;

	i = 0;
	is_first = 1;
	while (array[i] && array[i][0])
	{
		if (!is_first && array[i][0] == '\0')
		{
			i++;
			continue ;
		}
		while (!array[i][0] && array[i + 1])
			i++;
		result = handle_token_dispatch(array, &i, token, &is_first);
		if (result == -1)
			return ;
		if (result == 1)
			continue ;
		i++;
	}
}

void	tokenize(char **array, t_token **token)
{
	int	should_continue;

	validate_tokenize_input(array, token, &should_continue);
	if (should_continue)
		process_tokens(array, token);
}
