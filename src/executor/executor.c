/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:11:46 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 18:49:33 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_child_builtin(char *cmd_argv[MAX_ARGS], int cmd_argc)
{
	t_token	*cmd_token;
	int		k;

	if (!ft_strcmp(cmd_argv[0], "echo"))
	{
		cmd_token = NULL;
		create_token(&cmd_token, cmd_argv[0], "command");
		k = 1;
		while (k < cmd_argc)
		{
			create_token(&cmd_token, cmd_argv[k], "word");
			k++;
		}
		handle_echo_command(cmd_token);
		free_tokens(cmd_token);
	}
}

int prepare_child_command(t_token *seg, char *cmd_argv[MAX_ARGS])
{
	int cmd_argc = 0;

	while (seg && ft_strcmp(seg->type, "pipe") != 0)
	{
		if (ft_strncmp(seg->type, "redirect", 8) == 0
			|| ft_strcmp(seg->type, "append output") == 0)
		{
			if (setup_redirection(seg) == -1)
				return (-1);
			seg = seg->next;
			continue ;
		}
		else if (ft_strcmp(seg->type, "here-document") == 0)
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

char *find_executable(char *cmd, char **envp)
{
	char *path_env = NULL;
	char **paths = NULL;
	char *full_path = NULL;
	int i = 0;
	size_t len;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_env = envp[i] + 5;
			break;
		}
		i++;
	}
	if (!path_env)
		return NULL;
	paths = ft_split(path_env, ':');
	if (!paths)
		return NULL;

	i = 0;
	while (paths[i])
	{
		len = ft_strlen(paths[i]) + 1 + ft_strlen(cmd) + 1;
		full_path = malloc(len);
		if (!full_path)
			break;
		ft_strcpy(full_path, paths[i]);
		ft_strcat(full_path, "/");
		ft_strcat(full_path, cmd);

		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return full_path;
		}
		free(full_path);
		full_path = NULL;
		i++;
	}
	free_split(paths);
	return NULL;
}


void	execute_child_process(t_token *cmd_starts[256], int i, int heredoc_fds[256], 
							int pipefd[256][2], int num_cmds, char **envp)
{
	t_token *seg = cmd_starts[i];
	char *cmd_argv[MAX_ARGS];
	int cmd_argc;
	char *full_path;

	setup_child_signals();
	setup_child_pipes(pipefd, i, num_cmds);
	setup_child_heredoc(heredoc_fds, i);
	cmd_argc = prepare_child_command(seg, cmd_argv);
	if (cmd_argc == -1)
		exit(1);
	if (cmd_argc == 0 || cmd_argv[0] == NULL)
		exit(0);

	if (is_shell_builtin(cmd_argv[0]))
	{
		execute_child_builtin(cmd_argv, cmd_argc);
		exit(0);
	}
	if (cmd_argv[0][0] == '/' || cmd_argv[0][0] == '.')
		full_path = ft_strdup(cmd_argv[0]);
	else
		full_path = find_executable(cmd_argv[0], envp);
	if (!full_path)
	{
		perror(cmd_argv[0]);
		exit(127);
	}
	execve(full_path, cmd_argv, envp);
	perror(full_path);
	free(full_path);
	exit(127);
}

int	fork_processes(t_token *cmd_starts[256], int num_cmds, int heredoc_fds[256],
				int pipefd[256][2], pid_t pids[256], char **envp)
{
	int i = 0;

	while (i < num_cmds)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			perror("fork");
			set_shell_status(1);
			return (-1);
		}
		if (pids[i] == 0)
		{
			execute_child_process(cmd_starts, i, heredoc_fds, pipefd, num_cmds, envp);
		}
		i++;
	}
	return (0);
}
