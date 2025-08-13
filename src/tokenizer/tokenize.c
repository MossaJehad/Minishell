/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:30 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 05:15:36 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	create_token(t_token **token, char *value, char *type)
{
	t_token	*new;
	t_token	*temp;

	if (!value || !type)
	{
		printf("create_token error: NULL value/type\n");
		return ;
	}
	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	/* split those into a func */
	new->value = NULL;
	new->next = NULL;
	new->value = ft_strdup(value);
	if (!new->value || !new->type)
	{
		if (new->value)
			free(new->value);
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

//int	check_syntax_error(char **array)
//{
//	int	i;

//	i = 0;
//	while (array[i])
//	{
//		if (ft_strcmp(array[i], "|") == 0 || ft_strcmp(array[i], "<") == 0
//			|| ft_strcmp(array[i], ">") == 0 || ft_strcmp(array[i], ">>") == 0
//			|| ft_strcmp(array[i], "<<") == 0)
//		{
//			if (!array[++i])
//			{
//				printf("syntax error\n");
//				return (1);
//			}
//		}
//		i++;
//	}
//	return (0);
//}

//int	tokenize_append_and_heredoc(char **array, int *i, t_token **token)
//{
//	if (ft_strcmp(array[*i], ">>") == 0)
//	{
//		create_token(token, array[++(*i)], "append output");
//		(*i)++;
//		return (1);
//	}
//	else if (ft_strcmp(array[*i], "<<") == 0)
//	{
//		if (array[*i + 1] == NULL)
//		{
//			printf("syntax error: unexpected end after `<<`\n");
//			return (-1);
//		}
//		create_token(token, array[++(*i)], "here-document");
//		(*i)++;
//		return (1);
//	}
//	return (0);
//}

//int	tokenize_pipe_and_redirects(char **array, int *i, t_token **token)
//{
//	if (ft_strcmp(array[*i], "|") == 0)
//	{
//		create_token(token, "|", "pipe");
//		(*i)++;
//		return (1);
//	}
//	else if (ft_strcmp(array[*i], "<") == 0)
//	{
//		create_token(token, array[++(*i)], "redirect input");
//		(*i)++;
//		return (1);
//	}
//	else if (ft_strcmp(array[*i], ">") == 0)
//	{
//		create_token(token, array[++(*i)], "redirect output");
//		(*i)++;
//		return (1);
//	}
//	return (0);
//}

t_token	*tokenize(const char *input)
{
	t_token	*tokens;
	t_token	*new_token;
	char	*word;
	int		pos;
	int		is_first_word;

	if (!input)
		return (NULL);

	tokens = NULL;
	pos = 0;
	is_first_word = 1;

	while (input[pos])
	{
		skip_whitespace(input, &pos);
		
		if (!input[pos])
			break;

		/* Check for operators first */
		if (is_operator(input + pos))
		{
			new_token = tokenize_operator(input, &pos);
			if (new_token)
			{
				token_add_back(&tokens, new_token);
				is_first_word = (new_token->type == TOKEN_PIPE);
			}
		}
		else
		{
			/* Extract word */
			word = extract_word(input, &pos);
			if (word && *word)
			{
				new_token = token_new(word, determine_word_type(word, is_first_word));
				token_add_back(&tokens, new_token);
				is_first_word = 0;
			}
			free(word);
		}
	}

	return (tokens);
}
