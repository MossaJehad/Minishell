/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:11:46 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/27 16:23:29 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>

static void	run_echo_builtin(char *cmd_argv[MAX_ARGS], int cmd_argc)
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

static void	run_pwd_builtin(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
		perror("pwd");
}

static void	run_env_builtin(char **envp)
{
	handle_env_command(envp);
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
}

// Helpers for prepare_child_command
static int	handle_redirect_tokens(t_token **cur)
{
	if (setup_redirection(*cur) == -1)
		return (-1);
	*cur = (*cur)->next;
	if (*cur)
		*cur = (*cur)->next;
	return (0);
}

static void	skip_heredoc_tokens(t_token **cur)
{
	*cur = (*cur)->next;
	if (*cur)
		*cur = (*cur)->next;
}

int	prepare_child_command(t_token *seg, char *cmd_argv[MAX_ARGS])
{
	int		cmd_argc;
	t_token	*cur;

	cmd_argc = 0;
	cur = seg;
	while (cur && cur->type != PIPE)
	{
		if (cur->type == REDIRECT || cur->type == REDIRECT_OUT
			|| cur->type == APPEND)
		{
			if (handle_redirect_tokens(&cur) == -1)
				return (-1);
			continue ;
		}
		if (cur->type == HEREDOC)
		{
			skip_heredoc_tokens(&cur);
			continue ;
		}
		if (cur->type == WORD || cur->type == COMMAND
			|| cur->type == QUOTED_STRING)
			cmd_argv[cmd_argc++] = cur->value;
		cur = cur->next;
	}
	cmd_argv[cmd_argc] = NULL;
	return (cmd_argc);
}

// Helpers for find_executable
static char	*get_path_env(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*build_full_path(char *dir, char *cmd)
{
	size_t	len;
	char	*full;

	len = ft_strlen(dir) + 1 + ft_strlen(cmd) + 1;
	full = ft_calloc(len, sizeof(char));
	if (!full)
		return (NULL);
	ft_strcpy(full, dir);
	ft_strcat(full, "/");
	ft_strcat(full, cmd);
	return (full);
}

static char	*search_paths(char **paths, char *cmd)
{
	int		i;
	char	*full;

	i = 0;
	while (paths && paths[i])
	{
		full = build_full_path(paths[i], cmd);
		if (!full)
			return (NULL);
		if (access(full, X_OK) == 0)
			return (full);
		free(full);
		i++;
	}
	return (NULL);
}

char	*find_executable(char *cmd, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*full;

	path_env = get_path_env(envp);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	full = search_paths(paths, cmd);
	ft_free_arr(paths);
	return (full);
}

static int	child_setup_and_collect_args(t_exec_ctx *ctx, int idx,
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

static char	*resolve_cmd(char *cmd, char **envp)
{
	if (!cmd)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
		return (ft_strdup(cmd));
	return (find_executable(cmd, envp));
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
