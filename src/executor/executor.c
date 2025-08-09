/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:11:46 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 17:39:00 by mhasoneh         ###   ########.fr       */
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

void execute_child_process(t_token *cmd_starts[256], int i, int heredoc_fds[256], 
							int pipefd[256][2], int num_cmds)
{
	t_token	*seg = cmd_starts[i];
	char	*cmd_argv[MAX_ARGS];
	int		cmd_argc;

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

	execvp(cmd_argv[0], cmd_argv);
	perror(cmd_argv[0]);
	exit(127);
}

int	fork_processes(t_token *cmd_starts[256], int num_cmds, int heredoc_fds[256],
						int pipefd[256][2], pid_t pids[256])
{
	int	i;

	i = 0;
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
			execute_child_process(cmd_starts, i, heredoc_fds, pipefd, num_cmds);
		}
		i++;
	}
	return (0);
}
