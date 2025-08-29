/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:28:48 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/29 20:01:47 by mhasoneh         ###   ########.fr       */
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
