/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:11:46 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/29 17:07:13 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	run_echo_builtin(char *cmd_argv[MAX_ARGS], int cmd_argc)
{
	t_token	*cmd_token;
	int		k;

	cmd_token = NULL;
	create_token(&cmd_token, cmd_argv[0], "command");
	k = 1;
	while (k < cmd_argc)
		create_token(&cmd_token, cmd_argv[k++], "word");
	handle_echo_command(cmd_token);
	free_tokens(cmd_token);
}

void	run_pwd_builtin(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
		perror("pwd");
}

int	process_token_and_redirects(t_token **cur,
	char *cmd_argv[MAX_ARGS], int *cmd_argc)
{
	while (*cur && (*cur)->type != PIPE)
	{
		if ((*cur)->type == REDIRECT || (*cur)->type == REDIRECT_OUT
			|| (*cur)->type == APPEND)
		{
			if (handle_redirect_tokens(cur) == -1)
				return (-1);
			continue ;
		}
		if ((*cur)->type == HEREDOC)
		{
			skip_heredoc_tokens(cur);
			continue ;
		}
		if ((*cur)->type == WORD || (*cur)->type == COMMAND
			|| (*cur)->type == QUOTED_STRING)
			cmd_argv[(*cmd_argc)++] = (*cur)->value;
		*cur = (*cur)->next;
	}
	return (0);
}

void	execute_child_process(t_exec_ctx *ctx, int i, char **envp)
{
	char	*argv[MAX_ARGS];
	int		argc;
	char	*full;

	argc = child_setup_and_collect_args(ctx, i, argv);
	if (argc == -1)
		exit(1);
	if (argc == 0 || !argv[0])
		exit(0);
	if (is_builtin(argv[0]))
	{
		execute_child_builtin(argv, argc, envp);
		exit(0);
	}
	full = resolve_cmd(argv[0], envp);
	if (!full)
	{
		printf("%s: command not found\n", argv[0]);
		exit(127);
	}
	execve(full, argv, envp);
	perror(full);
	free(full);
	exit(127);
}

int	fork_processes(t_exec_ctx *ctx, char **envp)
{
	int	i;

	i = 0;
	while (i < ctx->cmd_count)
	{
		ctx->pids[i] = fork();
		if (ctx->pids[i] < 0)
		{
			perror("fork");
			set_shell_status(1);
			return (-1);
		}
		if (ctx->pids[i] == 0)
			execute_child_process(ctx, i, envp);
		i++;
	}
	return (0);
}
