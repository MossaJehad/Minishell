/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:30 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/10 11:09:54 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	create_token(t_token **token, char *value, char *type)
{
	t_token	*new;
	t_token	*temp;

	if (!value || !type)
	{
		fprintf(stderr, "create_token error: NULL value/type\n");
		return ;
	}
	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	new->value = ft_strdup(value);
	new->type = ft_strdup(type);
	if (!new->value || !new->type)
	{
		free(new->value);
		free(new->type);
		free(new);
		return ;
	}
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

int	tokenize_append_and_heredoc(char **array, int *i, t_token **token)
{
	if (ft_strcmp(array[*i], ">>") == 0)
	{
		create_token(token, array[++(*i)], "append output");
		(*i)++;
		return (1);
	}
	else if (ft_strcmp(array[*i], "<<") == 0)
	{
		if (array[*i + 1] == NULL)
		{
			fprintf(stderr, "syntax error: unexpected end after `<<`\n");
			return (-1);
		}
		create_token(token, array[++(*i)], "here-document");
		(*i)++;
		return (1);
	}
	return (0);
}

int	tokenize_pipe_and_redirects(char **array, int *i, t_token **token)
{
	if (ft_strcmp(array[*i], "|") == 0)
	{
		create_token(token, "|", "pipe");
		(*i)++;
		return (1);
	}
	else if (ft_strcmp(array[*i], "<") == 0)
	{
		create_token(token, array[++(*i)], "redirect input");
		(*i)++;
		return (1);
	}
	else if (ft_strcmp(array[*i], ">") == 0)
	{
		create_token(token, array[++(*i)], "redirect output");
		(*i)++;
		return (1);
	}
	return (0);
}

void	tokenize(char **array, t_token **token)
{
	int	result;
	int	i;

	i = 0;
	while (array[i])
	{
		result = tokenize_append_and_heredoc(array, &i, token);
		if (result == -1)
			return ;
		if (result == 1)
			continue ;
		result = tokenize_pipe_and_redirects(array, &i, token);
		if (result == 1)
			continue ;
		if (check_command(array[i]))
			create_token(token, array[i], "command");
		else
			create_token(token, array[i], "word");
		i++;
	}
}
