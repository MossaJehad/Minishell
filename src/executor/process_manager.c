/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:19:05 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 05:28:24 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_single_builtin(char *cmd_argv[MAX_ARGS], int cmd_argc,
		char ***envp)
{
	if (!ft_strcmp(cmd_argv[0], "export"))
		handle_export_command(envp, cmd_argv, cmd_argc);
	else if (!ft_strcmp(cmd_argv[0], "unset"))
		handle_unset_command(envp, cmd_argv, cmd_argc);
	else if (!ft_strcmp(cmd_argv[0], "cd"))
		handle_cd_command(cmd_argv[1], cmd_argc, envp);
	else if (!ft_strcmp(cmd_argv[0], "exit"))
		handle_exit_command(cmd_argv, cmd_argc);
}

//void	init_heredoc_fds(int heredoc_fds[256])
//{
//	int	k;

//	k = 0;
//	while (k < 256)
//	{
//		heredoc_fds[k] = -1;
//		k++;
//	}
//}

//int	setup_heredocs_for_cmd(t_token *cmd_start, int cmd_index,
//		int heredoc_fds[256])
//{
//	t_token	*temp;

//	temp = cmd_start;
//	while (temp && ft_strcmp(temp->type, "pipe") != 0)
//	{
//		if (ft_strcmp(temp->type, "here-document") == 0)
//		{
//			heredoc_fds[cmd_index] = setup_redirection(temp);
//			if (heredoc_fds[cmd_index] == -1)
//			{
//				perror("heredoc failed");
//				set_shell_status(1);
//				return (-1);
//			}
//		}
//		temp = temp->next;
//	}
//	return (0);
//}

void	wait_for_processes(pid_t pids[256], int num_cmds)
{
	int	i;
	int	status;
	int	sig;

	i = 0;
	while (i < num_cmds)
	{
		waitpid(pids[i], &status, 0);
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

t_cmd	*parse_commands(t_token *tokens)
{
	t_cmd	*commands;
	t_cmd	*current_cmd;
	t_token	*start;
	t_token	*current;

	if (!tokens)
		return (NULL);

	commands = NULL;
	start = tokens;
	current = tokens;

	while (current)
	{
		if (current->type == TOKEN_PIPE || !current->next)
		{
			current_cmd = cmd_new();
			
			/* Setup redirections first */
			if (!setup_redirections(current_cmd, start, current->next))
			{
				cmd_free(current_cmd);
				commands_free(commands);
				return (NULL);
			}
			
			/* Fill arguments */
			fill_args(current_cmd, start, current->next);
			
			cmd_add_back(&commands, current_cmd);
			
			if (current->type == TOKEN_PIPE)
				start = current->next;
		}
		current = current->next;
	}

	return (commands);
}


int	validate_syntax(t_token *tokens)
{
	t_token	*current;
	int		expect_word;

	if (!tokens)
		return (1);

	current = tokens;
	expect_word = 0;

	while (current)
	{
		if (expect_word)
		{
			if (current->type != TOKEN_WORD && current->type != TOKEN_COMMAND)
			{
				print_error("minishell", NULL, "syntax error: expected filename");
				return (0);
			}
			expect_word = 0;
		}
		else if (current->type == TOKEN_PIPE)
		{
			if (!current->next)
			{
				print_error("minishell", NULL, "syntax error: unexpected end after '|'");
				return (0);
			}
		}
		else if (current->type == TOKEN_REDIRECT_IN || 
				 current->type == TOKEN_REDIRECT_OUT ||
				 current->type == TOKEN_REDIRECT_APPEND ||
				 current->type == TOKEN_HEREDOC)
		{
			expect_word = 1;
		}
		current = current->next;
	}

	if (expect_word)
	{
		print_error("minishell", NULL, "syntax error: expected filename");
		return (0);
	}

	return (1);
}