/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:30 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/20 02:43:12 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	find_type(t_token	*new, char *type)
{
	if (ft_strcmp(type, "command") == 0)
		new->type = WORD;
	else if (ft_strcmp(type, "word") == 0)
		new->type = WORD;
	else if (ft_strcmp(type, "pipe") == 0)
		new->type = PIPE;
	else if (ft_strcmp(type, "REDIRIN input") == 0)
		new->type = REDIRIN;
	else if (ft_strcmp(type, "REDIRIN output") == 0)
		new->type = REDIROUT;
	else if (ft_strcmp(type, "append output") == 0)
		new->type = APPEND;
	else if (ft_strcmp(type, "here-document") == 0)
		new->type = HEREDOC;
	else
		new->type = UNKNOWN;
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

int	check_syntax_error(char **array)
{
	int	i;

	if (!array || !array[0])
		return (0);
	i = 0;
	while (array[i])
	{
		if (is_operator(array[i]))
		{
			if (!array[i + 1] || is_operator(array[i + 1]))
			{
				printf("minishell: syntax error near unexpected token `%s'\n", 
					array[i + 1] ? array[i + 1] : "newline");
					ft_free_arr(array);
				return (1);
			}
		}
		if (ft_strcmp(array[i], "|") == 0)
		{
			if (i == 0)
			{
				printf("minishell: syntax error near unexpected token `|'\n");
				ft_free_arr(array);
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
		create_token(token, ">>", "append output");
		(*i)++;
		if (!array[*i])
		{
			printf("syntax error: unexpected end after `>>`\n");
			return (-1);
		}
		create_token(token, array[*i], "word");
		(*i)++;
		return (1);
	}
	else if (ft_strcmp(array[*i], "<<") == 0)
	{
		create_token(token, "<<", "here-document");
		(*i)++;
		if (!array[*i])
		{
			printf("syntax error: unexpected end after `<<`\n");
			return (-1);
		}
		create_token(token, array[*i], "word");
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
		create_token(token, "<", "REDIRIN input");
		(*i)++;
		if (!array[*i])
		{
			printf("syntax error: unexpected end after `<`\n");
			return (-1);
		}
		create_token(token, array[*i], "word");
		(*i)++;
		return (1);
	}
	else if (ft_strcmp(array[*i], ">") == 0)
	{
		create_token(token, ">", "REDIRIN output");
		(*i)++;
		if (!array[*i])
		{
			printf("syntax error: unexpected end after `>`\n");
			return (-1);
		}
		create_token(token, array[*i], "word");
		(*i)++;
		return (1);
	}
	return (0);
}

void	tokenize(char **array, t_token **token)
{
	int	result;
	int	i;

	if (!array || !token)
		return ;
	i = 0;
	while (array[i])
	{
		while (!array[i][0])
			i++;
		result = tokenize_append_and_heredoc(array, &i, token);
		if (result == -1)
		{
			free_tokens(*token);
			*token = NULL;
			return ;
		}
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
