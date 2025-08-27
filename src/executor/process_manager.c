/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:19:05 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/27 16:26:28 by mhasoneh         ###   ########.fr       */
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

int	setup_heredocs_for_cmd(t_token *cmd_start, int cmd_index,
		int heredoc_fds[MAX_CMDS])
{
	t_token	*temp;
	int		new_fd;

	temp = cmd_start;
	while (temp && (temp->type != PIPE))
	{
		if (temp->type == HEREDOC)
		{
			if (heredoc_fds[cmd_index] != -1)
			{
				close(heredoc_fds[cmd_index]);
				heredoc_fds[cmd_index] = -1;
			}
			new_fd = setup_redirection(temp);
			if (new_fd == -1)
			{
				perror("heredoc failed");
				set_shell_status(1);
				return (-1);
			}
			heredoc_fds[cmd_index] = new_fd;
		}
		temp = temp->next;
	}
	return (0);
}

void	sig_check(int status)
{
	int		sig;

	sig = WTERMSIG(status);
	if (sig == SIGINT)
		set_shell_status(130);
	else if (sig == SIGQUIT)
	{
		set_shell_status(131);
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	}
	else
		set_shell_status(128 + sig);
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

void	wait_for_processes(pid_t pids[256], int num_cmds)
{
	int		i;

	i = 0;
	while (i < num_cmds)
	{
		handle_process_status(pids[i], i, num_cmds);
		i++;
	}
	setup_signal_handlers();
}
