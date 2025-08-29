/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:00:53 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/29 17:10:44 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	run_exit_builtin_child(char *cmd_argv[MAX_ARGS], int cmd_argc)
{
	if (cmd_argc == 1)
		exit(get_shell_status());
	else if (cmd_argc == 2)
		handle_exit_argument(cmd_argv[1]);
	else
	{
		if (!is_valid_number(cmd_argv[1]) || check_overflow(cmd_argv[1]))
		{
			printf("minishell: exit: %s: numeric argument required\n",
				cmd_argv[1]);
			exit(2);
		}
		else
		{
			printf("minishell: exit: too many arguments\n");
			exit(1);
		}
	}
}

void	execute_child_builtin(char *cmd_argv[MAX_ARGS], int cmd_argc,
		char **envp)
{
	if (!ft_strcmp(cmd_argv[0], "echo"))
		run_echo_builtin(cmd_argv, cmd_argc);
	else if (!ft_strcmp(cmd_argv[0], "pwd"))
		run_pwd_builtin();
	else if (!ft_strcmp(cmd_argv[0], "env"))
		run_env_builtin(envp);
	else if (!ft_strcmp(cmd_argv[0], "exit"))
		run_exit_builtin_child(cmd_argv, cmd_argc);
}

// Helpers for prepare_child_command
int	handle_redirect_tokens(t_token **cur)
{
	if (setup_redirection(*cur) == -1)
		return (-1);
	*cur = (*cur)->next;
	if (*cur)
		*cur = (*cur)->next;
	return (0);
}

int	prepare_child_command(t_token *seg, char *cmd_argv[MAX_ARGS])
{
	int		cmd_argc;
	t_token	*cur;

	cmd_argc = 0;
	cur = seg;
	if (process_token_and_redirects(&cur, cmd_argv, &cmd_argc) == -1)
		return (-1);
	cmd_argv[cmd_argc] = NULL;
	return (cmd_argc);
}

int	child_setup_and_collect_args(t_exec_ctx *ctx, int idx,
	char *cmd_argv[MAX_ARGS])
{
	t_token	*seg;

	seg = ctx->cmd_starts[idx];
	setup_child_signals();
	setup_child_pipes(ctx->pipe_fds, idx, ctx->cmd_count);
	setup_child_heredoc(ctx->heredoc_fds, idx);
	close_heredoc_fds(ctx->heredoc_fds, ctx->cmd_count);
	return (prepare_child_command(seg, cmd_argv));
}
