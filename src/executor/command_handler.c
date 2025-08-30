/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malja-fa <malja-fa@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:14:14 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/30 10:41:39 by malja-fa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	setup_pipes_and_fork(t_exec_ctx *ctx, char ***envp)
{
	if (create_pipes(ctx->pipe_fds, ctx->cmd_count) == -1)
	{
		close_heredoc_fds(ctx->heredoc_fds, MAX_CMDS);
		restore_signals();
		return (-1);
	}
	if (fork_processes(ctx, *envp) == -1)
	{
		close_all_pipes(ctx->pipe_fds, ctx->cmd_count);
		close_heredoc_fds(ctx->heredoc_fds, MAX_CMDS);
		restore_signals();
		return (-1);
	}
	return (0);
}

int	prepare_and_execute_commands(t_token *token, char ***envp, t_exec_ctx *ctx)
{
	ctx->cmd_count = parse_commands(token, ctx->cmd_starts, ctx->heredoc_fds);
	if (ctx->cmd_count == -1)
	{
		close_heredoc_fds(ctx->heredoc_fds, MAX_CMDS);
		return (-1);
	}
	if (ctx->cmd_count == 1)
	{
		if (handle_cmd_count_one(ctx, envp) == 0)
			return (0);
	}
	ignore_signals();
	if (setup_pipes_and_fork(ctx, envp) == -1)
		return (-1);
	return (0);
}

void	finalize_command_execution(t_exec_ctx *ctx)
{
	close_all_pipes(ctx->pipe_fds, ctx->cmd_count);
	close_heredoc_fds(ctx->heredoc_fds, MAX_CMDS);
	wait_for_processes(ctx->pids, ctx->cmd_count);
	restore_signals();
}

void	handle_command(t_token *token, char ***envp)
{
	t_exec_ctx	ctx;
	int			i;

	memset(&ctx, 0, sizeof(ctx));
	i = 0;
	while (i < MAX_CMDS)
		ctx.heredoc_fds[i++] = -1;
	token->ctx = &ctx;
	if (prepare_and_execute_commands(token, envp, &ctx) == 0)
		finalize_command_execution(&ctx);
	else
		close_heredoc_fds(ctx.heredoc_fds, MAX_CMDS);
}

void	close_heredoc_fds(int heredoc_fds[MAX_CMDS], int num_cmds)
{
	int	i;

	(void)num_cmds;
	i = 0;
	while (i < MAX_CMDS)
	{
		if (heredoc_fds[i] != -1)
		{
			close(heredoc_fds[i]);
			heredoc_fds[i] = -1;
		}
		i++;
	}
}
