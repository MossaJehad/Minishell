/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:19:05 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/20 02:58:39 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_single_builtin(t_shell *shell, char *cmd_argv[MAX_ARGS],
			int cmd_argc, char ***envp)
{
	if (!ft_strcmp(cmd_argv[0], "export"))
		ft_export(shell);
	else if (!ft_strcmp(cmd_argv[0], "unset"))
		handle_unset_command(envp, cmd_argv, cmd_argc);
	else if (!ft_strcmp(cmd_argv[0], "cd"))
		handle_cd_command(cmd_argv[1], cmd_argc, envp);
	else if (!ft_strcmp(cmd_argv[0], "exit"))
		ft_exit(shell, cmd_argv);
}

void	init_heredoc_fds(int heredoc_fds[MAX_CMDS])
{
	int	k;

	k = 0;
	while (k < MAX_CMDS)
	{
		heredoc_fds[k] = -1;
		k++;
	}
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
			heredoc_fds[cmd_index] = setup_redirection(temp);
			if (heredoc_fds[cmd_index] == -1)
			{
				perror("heredoc failed");
				set_shell_status(1);
				return (-1);
			}
		}
		temp = temp->next;
	}
	return (0);
}

void	wait_for_processes(pid_t pids[256], int num_cmds)
{
	int		i;
	int		status;
	int		sig;
	pid_t	ret;

	i = 0;
	while (i < num_cmds)
	{
		status = 0;
		ret = waitpid(pids[i], &status, 0);
		if (ret == -1)
		{
			i++;
			continue;
		}
		if (i == num_cmds - 1)
		{
			if (WIFEXITED(status))
				set_shell_status(WEXITSTATUS(status));
			else if (WIFSIGNALED(status))
			{
				sig = WTERMSIG(status);
				if (sig == SIGINT)
					set_shell_status(130);
				else if (sig == SIGQUIT)
					set_shell_status(131);
				else
					set_shell_status(128 + sig);
			}
		}
		i++;
	}
}

int	parse_commands(t_token *token, t_token *cmd_starts[256], int heredoc_fds[256])
{
	int		num_cmds;
	t_token	*current;

	num_cmds = 0;
	current = token;
	init_heredoc_fds(heredoc_fds);
	if (!current)
		return (0);
	cmd_starts[num_cmds] = current;
	if (setup_heredocs_for_cmd(cmd_starts[num_cmds], num_cmds, heredoc_fds) == -1)
		return (-1);
	num_cmds++;
	while (current)
	{
		if (current->type == PIPE)
		{
			if (!current->next)
			{
				printf("syntax error: unexpected end after pipe\n");
				return (-1);
			}
			cmd_starts[num_cmds] = current->next;
			if (setup_heredocs_for_cmd(cmd_starts[num_cmds], num_cmds, heredoc_fds) == -1)
				return (-1);
			num_cmds++;
		}
		current = current->next;
	}
	return (num_cmds);
}
