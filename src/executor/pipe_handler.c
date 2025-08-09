/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:19:26 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 17:19:56 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_single_command(t_token *cmd_starts[256], int heredoc_fds[256], char ***envp)
{
	t_token	*seg;
	int		cmd_argc;
	char	*cmd_argv[MAX_ARGS];

	seg = cmd_starts[0];
	cmd_argc = build_cmd_args(seg, cmd_argv);
	if (is_shell_builtin(cmd_argv[0]) && should_run_in_parent(cmd_argv[0]))
	{
		handle_single_builtin(cmd_argv, cmd_argc, envp);
		if (heredoc_fds[0] != -1)
			close(heredoc_fds[0]);
		return (1);
	}
	return (0);
}

int	create_pipes(int pipefd[256][2], int num_cmds)
{
	int	i;

	i = 0;
	while (i < num_cmds - 1)
	{
		if (pipe(pipefd[i]) < 0)
		{
			perror("pipe");
			set_shell_status(1);
			return (-1);
		}
		i++;
	}
	return (0);
}

void	close_all_pipes(int pipefd[256][2], int num_cmds)
{
	int	j;

	j = 0;
	while (j < num_cmds - 1)
	{
		close(pipefd[j][0]);
		close(pipefd[j][1]);
		j++;
	}
}

void	setup_child_pipes(int pipefd[256][2], int i, int num_cmds)
{
	if (i > 0)
		dup2(pipefd[i - 1][0], STDIN_FILENO);
	if (i < num_cmds - 1)
		dup2(pipefd[i][1], STDOUT_FILENO);
	close_all_pipes(pipefd, num_cmds);
}

void	setup_child_heredoc(int heredoc_fds[256], int i)
{
	if (heredoc_fds[i] != -1)
	{
		dup2(heredoc_fds[i], STDIN_FILENO);
		close(heredoc_fds[i]);
	}
}
