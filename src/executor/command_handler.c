/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:14:14 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 20:53:46 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	prepare_and_execute_commands(t_token *token, char ***envp, t_exec_ctx *ctx)
{
	ctx->num_cmds = parse_commands(token, ctx->cmd_starts, ctx->heredoc_fds);
	if (ctx->num_cmds == -1)
		return (-1);
	if (ctx->num_cmds == 1)
	{
		if (handle_single_command(ctx->cmd_starts, ctx->heredoc_fds, envp))
			return (0);
	}
	ignore_signals();
	if (create_pipes(ctx->pipefd, ctx->num_cmds) == -1
		|| fork_processes(ctx->cmd_starts, ctx->num_cmds, ctx->heredoc_fds,
			ctx->pipefd, ctx->pids, *envp) == -1)
	{
		restore_signals();
		return (-1);
	}
	return (0);
}

void	finalize_command_execution(t_exec_ctx *ctx)
{
	close_all_pipes(ctx->pipefd, ctx->num_cmds);
	close_heredoc_fds(ctx->heredoc_fds, ctx->num_cmds);
	wait_for_processes(ctx->pids, ctx->num_cmds);
	restore_signals();
}

void	handle_command(t_token *token, char ***envp)
{
	t_exec_ctx	ctx;

	if (prepare_and_execute_commands(token, envp, &ctx) == 0)
	{
		finalize_command_execution(&ctx);
	}
}

int	build_cmd_args(t_token *seg, char *cmd_argv[MAX_ARGS])
{
	int	cmd_argc;

	cmd_argc = 0;
	while (seg && ft_strcmp(seg->type, "pipe") != 0)
	{
		if (ft_strncmp(seg->type, "redirect", 8) == 0 || ft_strcmp(seg->type,
				"here-document") == 0 || ft_strcmp(seg->type,
				"append output") == 0)
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

void	close_heredoc_fds(int heredoc_fds[256], int num_cmds)
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
