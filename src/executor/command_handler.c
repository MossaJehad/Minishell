/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:14:14 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/20 02:39:32 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	prepare_and_execute_commands(t_token *token, char ***envp, t_exec_ctx *ctx, t_shell *shell)
{
	ctx->cmd_count = parse_commands(token, ctx->cmd_starts, ctx->heredoc_fds);
	if (ctx->cmd_count == -1)
		return (-1);
	if (ctx->cmd_count == 1)
	{
		if (handle_single_command(ctx->cmd_starts, ctx->heredoc_fds, envp, shell))
			return (0);
	}
	ignore_signals();
	if (create_pipes(ctx->pipe_fds, ctx->cmd_count) == -1
		|| fork_processes(ctx->cmd_starts, ctx->cmd_count, ctx->heredoc_fds,
			ctx->pipe_fds, ctx->pids, *envp) == -1)
	{
		restore_signals();
		return (-1);
	}
	return (0);
}

void	finalize_command_execution(t_exec_ctx *ctx)
{
	close_all_pipes(ctx->pipe_fds, ctx->cmd_count);
	close_heredoc_fds(ctx->heredoc_fds, ctx->cmd_count);
	wait_for_processes(ctx->pids, ctx->cmd_count);
	restore_signals();
}

void	handle_command(t_token *token, char ***envp, t_shell *shell)
{
	t_exec_ctx	ctx;
	t_token		*current;
	int			is_heredoc_only;

	ft_bzero(&ctx, sizeof(t_exec_ctx));
	is_heredoc_only = 1;
	current = token;
	while (current)
	{
		if (current->type == WORD)
		{
			is_heredoc_only = 0;
			break;
		}
		current = current->next;
	}
	if (is_heredoc_only)
	{
		current = token;
		while (current)
		{
			if (current->type == HEREDOC)
			{
				int heredoc_fd = handle_heredoc(current->next, ctx);
				if (heredoc_fd != -1)
					close(heredoc_fd);
				return ;
			}
			current = current->next;
		}
		return ;
	}
	if (prepare_and_execute_commands(token, envp, &ctx, shell) == 0)
		finalize_command_execution(&ctx);
}

int	build_cmd_args(t_token *seg, char *cmd_argv[MAX_ARGS])
{
	int	cmd_argc;

	cmd_argc = 0;
	while (seg && seg->type != PIPE)
	{
		if (seg->type == REDIRIN || seg->type == HEREDOC || seg->type == APPEND)
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

void	close_heredoc_fds(int heredoc_fds[MAX_CMDS], int num_cmds)
{
	int	i;

	i = 0;
	while (i < num_cmds)
	{
		if (heredoc_fds[i] != -1)
			close(heredoc_fds[i]);
		i++;
	}
}
