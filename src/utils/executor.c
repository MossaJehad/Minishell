/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:37:19 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/29 16:38:56 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_cmd_count_one(t_exec_ctx *ctx, char ***envp)
{
	if (handle_single_command(ctx->cmd_starts, ctx->heredoc_fds, envp))
	{
		close_heredoc_fds(ctx->heredoc_fds, MAX_CMDS);
		return (0);
	}
	return (-1);
}

int	build_cmd_args(t_token *seg, char *cmd_argv[MAX_ARGS])
{
	int	cmd_argc;

	cmd_argc = 0;
	while (seg && seg->type != PIPE)
	{
		if (seg->type == REDIRECT || seg->type == HEREDOC
			|| seg->type == APPEND)
		{
			seg = seg->next;
			continue ;
		}
		cmd_argv[cmd_argc++] = seg->value;
		seg = seg->next;
	}
	cmd_argv[cmd_argc] = NULL;
	return (cmd_argc);
}

void	sig_check(int status)
{
	int	sig;

	sig = WTERMSIG(status);
	if (sig == SIGINT)
		set_shell_status(130);
	else if (sig == SIGQUIT)
	{
		set_shell_status(131);
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	}
	else
		set_shell_status(128 + sig);
}
