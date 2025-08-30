/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:11:46 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/30 16:01:47 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_child_args(t_exec_ctx *ctx, char **argv,
	int *argc, char **envp)
{
	*argc = child_setup_and_collect_args(ctx, ctx->extra_counter, argv);
	if (*argc == -1)
	{
		cleanup_shell_resources(&envp, *ctx->cmd_starts, NULL, NULL);
		exit(1);
	}
	if (*argc == 0 || !argv[0])
	{
		cleanup_shell_resources(&envp, *ctx->cmd_starts, NULL, NULL);
		exit(0);
	}
}

void	handle_builtin_if_needed(char **argv, int argc,
	char **envp, t_exec_ctx *ctx)
{
	if (is_builtin(argv[0]))
	{
		execute_child_builtin(argv, argc, envp, ctx);
		cleanup_shell_resources(&envp, *ctx->cmd_starts, NULL, NULL);
		exit(0);
	}
}

void	execute_external_cmd(char **argv, char **envp, t_exec_ctx *ctx)
{
	char	*full;

	full = resolve_cmd(argv[0], envp);
	if (!full)
	{
		if (is_dir(argv[0], 1) == 126)
		{
			cleanup_shell_resources(&envp, *ctx->cmd_starts, NULL, full);
			exit(126);
		}
		write(2, argv[0], ft_strlen(argv[0]));
		write(2, ": command not found\n", 21);
		cleanup_shell_resources(&envp, *ctx->cmd_starts, NULL, full);
		exit(127);
	}
	execve(full, argv, envp);
	cleanup_shell_resources(&envp, *ctx->cmd_starts, NULL, NULL);
	perror(full);
	if (access(full, F_OK) == 0)
	{
		free(full);
		exit(126);
	}
	free(full);
	exit(127);
}

void	execute_child_process(t_exec_ctx *ctx, char **envp)
{
	char	*argv[MAX_ARGS];
	int		argc;

	handle_child_args(ctx, argv, &argc, envp);
	handle_builtin_if_needed(argv, argc, envp, ctx);
	execute_external_cmd(argv, envp, ctx);
}

int	fork_processes(t_exec_ctx *ctx, char **envp)
{
	ctx->extra_counter = 0;
	while (ctx->extra_counter < ctx->cmd_count)
	{
		ctx->pids[ctx->extra_counter] = fork();
		if (ctx->pids[ctx->extra_counter] < 0)
		{
			perror("fork");
			set_shell_status(1);
			return (-1);
		}
		if (ctx->pids[ctx->extra_counter] == 0)
			execute_child_process(ctx, envp);
		ctx->extra_counter++;
	}
	return (0);
}
