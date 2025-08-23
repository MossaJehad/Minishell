/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:21:19 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/23 20:21:31 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	init_command_parsing(t_token *token, t_token *cmd_starts[256],
		int heredoc_fds[256], int *num_cmds)
{
	*num_cmds = 0;
	init_heredoc_fds(heredoc_fds);
	if (!token)
		return (0);
	cmd_starts[*num_cmds] = token;
	if (setup_heredocs_for_cmd(cmd_starts[*num_cmds],
			*num_cmds, heredoc_fds) == -1)
		return (-1);
	(*num_cmds)++;
	return (1);
}

int	process_piped_commands(t_token *token, t_token *cmd_starts[256],
		int heredoc_fds[256], int *num_cmds)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (current->type == PIPE)
		{
			if (!current->next)
			{
				printf("syntax error: unexpected end after pipe\n");
				return (-1);
			}
			cmd_starts[*num_cmds] = current->next;
			if (setup_heredocs_for_cmd(cmd_starts[*num_cmds], *num_cmds,
					heredoc_fds) == -1)
				return (-1);
			(*num_cmds)++;
		}
		current = current->next;
	}
	return (*num_cmds);
}

int	parse_commands(t_token *token, t_token *cmd_starts[256],
		int heredoc_fds[256])
{
	int	num_cmds;
	int	result;

	result = init_command_parsing(token, cmd_starts, heredoc_fds, &num_cmds);
	if (result <= 0)
		return (result);
	return (process_piped_commands(token, cmd_starts, heredoc_fds, &num_cmds));
}
