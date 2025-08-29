/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:19:05 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/29 16:38:52 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_single_builtin(char *cmd_argv[MAX_ARGS], t_token *seg,
		int cmd_argc, char ***envp)
{
	if (!ft_strcmp(cmd_argv[0], "export"))
		handle_export_command(envp, cmd_argv, cmd_argc);
	else if (!ft_strcmp(cmd_argv[0], "unset"))
		handle_unset_command(envp, cmd_argv, cmd_argc);
	else if (!ft_strcmp(cmd_argv[0], "cd"))
		handle_cd_command(cmd_argv[1], cmd_argc, envp);
	else if (!ft_strcmp(cmd_argv[0], "exit"))
		handle_exit_command(cmd_argv, seg, cmd_argc, envp);
}

static int	handle_heredoc_token(t_token *temp,
		int cmd_index, int heredoc_fds[MAX_CMDS])
{
	int	new_fd;

	if (heredoc_fds[cmd_index] != -1)
	{
		close(heredoc_fds[cmd_index]);
		heredoc_fds[cmd_index] = -1;
	}
	new_fd = handle_heredoc(temp->next, temp->ctx);
	if (new_fd == 130)
		return (-1);
	if (new_fd == -1)
	{
		perror("warning: here-document delimited by EOF");
		set_shell_status(1);
		return (-1);
	}
	heredoc_fds[cmd_index] = new_fd;
	return (0);
}

int	setup_heredocs_for_cmd(t_token *cmd_start, int cmd_index,
		int heredoc_fds[MAX_CMDS])
{
	t_token	*temp;

	temp = cmd_start;
	while (temp && (temp->type != PIPE))
	{
		if (temp->type == HEREDOC)
		{
			if (handle_heredoc_token(temp, cmd_index, heredoc_fds) != 0)
				return (-1);
		}
		temp = temp->next;
	}
	return (0);
}

void	handle_process_status(pid_t pid, int index, int num_cmds)
{
	int		status;
	pid_t	ret;

	status = 0;
	ret = waitpid(pid, &status, 0);
	if (ret == -1)
		return ;
	if (index == num_cmds - 1)
	{
		if (WIFEXITED(status))
			set_shell_status(WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			sig_check(status);
	}
}

void	wait_for_processes(pid_t pids[MAX_CMDS], int num_cmds)
{
	int	i;

	i = 0;
	while (i < num_cmds)
	{
		if (pids[i] > 0)
			handle_process_status(pids[i], i, num_cmds);
		i++;
	}
	setup_signal_handlers();
}
