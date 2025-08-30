/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:28:48 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/30 15:59:12 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_valid_identifier(const char *str)
{
	size_t	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	print_ctx(t_exec_ctx *ctx)
{
	int		i;
	t_token	*tok;

	i = 0;
	printf("Context debug:\n");
	printf("cmd_count = %d\n", ctx->cmd_count);
	while (i < ctx->cmd_count)
	{
		printf("  Command %d:\n", i);
		tok = ctx->cmd_starts[i];
		while (tok)
		{
			printf("    token: \"%s\" (type=%d)\n", tok->value, tok->type);
			tok = tok->next;
		}
		i++;
	}
}

int	search_type(t_exec_ctx *ctx)
{
	int			i;
	int			counter;
	t_exec_ctx	holder;

	holder.cmd_count = ctx->cmd_count;
	holder.cmd_starts[0] = ctx->cmd_starts[0];
	i = 0;
	counter = 0;
	while (i < ctx->cmd_count)
	{
		while (ctx->cmd_starts[i])
		{
			if (ctx->cmd_starts[i]->type == 0)
				counter++;
			else
				counter--;
			ctx->cmd_starts[i] = ctx->cmd_starts[i]->next;
		}
		i++;
	}
	ctx->cmd_count = holder.cmd_count;
	ctx->cmd_starts[0] = holder.cmd_starts[0];
	return (counter);
}

void	find_type(t_token *newt, char *type)
{
	if (ft_strcmp(type, "command") == 0)
		newt->type = COMMAND;
	else if (ft_strcmp(type, "word") == 0)
		newt->type = WORD;
	else if (ft_strcmp(type, "pipe") == 0)
		newt->type = PIPE;
	else if (ft_strcmp(type, "redirect input") == 0)
		newt->type = REDIRECT;
	else if (ft_strcmp(type, "redirect output") == 0)
		newt->type = REDIRECT_OUT;
	else if (ft_strcmp(type, "append output") == 0)
		newt->type = APPEND;
	else if (ft_strcmp(type, "here-document") == 0)
		newt->type = HEREDOC;
	else
		newt->type = WORD;
}

int	count_split_args(char **args)
{
	int		total;
	int		i;
	int		count;

	total = 0;
	i = 0;
	while (args[i] != NULL)
	{
		if (ft_strcmp(args[i], "\001") == 0)
		{
			i++;
			continue ;
		}
		if (!is_quoted_expansion(args[i]) && ft_strchr(args[i], ' '))
		{
			count = count_words_from_split(args[i]);
			total += count;
		}
		else
		{
			total++;
		}
		i++;
	}
	return (total);
}
