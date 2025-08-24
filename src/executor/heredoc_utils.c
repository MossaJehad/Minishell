/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:15:57 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/24 17:22:18 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_heredoc_only(t_token *token)
{
	t_token		*current;
	int			is_heredoc_only;

	is_heredoc_only = 1;
	current = token;
	while (current)
	{
		if (current->type == COMMAND || current->type == WORD)
		{
			is_heredoc_only = 0;
			break ;
		}
		current = current->next;
	}
	return (is_heredoc_only);
}

void	process_heredoc_tokens(t_token *token, t_exec_ctx *ctx)
{
	t_token		*current;
	int			heredoc_fd;

	current = token;
	while (current)
	{
		if (current->type == HEREDOC)
		{
			heredoc_fd = handle_heredoc(current->next, *ctx);
			if (heredoc_fd != -1)
				close(heredoc_fd);
			return ;
		}
		current = current->next;
	}
}

void	check_heredoc_only(t_token *token, t_exec_ctx *ctx)
{
	if (is_heredoc_only(token))
		process_heredoc_tokens(token, ctx);
}

void	init_heredoc_fds(int heredoc_fds[MAX_CMDS])
{
	int	k;

	k = 0;
	while (k < MAX_CMDS)
	{
		heredoc_fds[k] = -1;
		k++;
	}
}
