/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:18 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/08 23:07:59 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
extern t_shell g_shell;

int	setup_redirection(t_token *tok)
{
	int		pipefd[2];
	char	*line;
	int		fd;

	if (!tok)
	{
		fprintf(stderr, "setup_redirection: null token\n");
		return (-1);
	}
	if (!tok->type)
	{
		fprintf(stderr, "setup_redirection: token type is null\n");
		return (-1);
	}
	if (ft_strcmp(tok->type, "here-document") == 0 && !tok->value)
	{
		fprintf(stderr, "heredoc: missing delimiter\n");
		return (-1);
	}
	if (ft_strcmp(tok->type, "here-document") == 0)
	{
		if (pipe(pipefd) == -1)
			return (perror("pipe"), -1);
		signal(SIGINT, SIG_DFL);
		while (1)
		{
			line = readline("> ");
			if (!line)
			{
				printf("\n");
				break ;
			}
			if (ft_strcmp(line, tok->value) == 0)
			{
				free(line);
				break ;
			}
			if (write(pipefd[1], line, ft_strlen(line)) == -1
				|| write(pipefd[1], "\n", 1) == -1)
			{
				perror("write to heredoc pipe");
				free(line);
				close(pipefd[0]);
				close(pipefd[1]);
				return (-1);
			}
			free(line);
		}
		close(pipefd[1]);
		setup_signal_handlers();
		return (pipefd[0]);
	}
	if (ft_strcmp(tok->type, "redirect input") == 0)
		fd = open(tok->value, O_RDONLY);
	else if (ft_strcmp(tok->type, "redirect output") == 0)
		fd = open(tok->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (ft_strcmp(tok->type, "append output") == 0)
		fd = open(tok->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (-1);
	if (fd < 0)
		return (perror(tok->value), -1);
	if (ft_strcmp(tok->type, "redirect input") == 0)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

void	handle_command(t_token *token, char ***envp)
{
	int		num_cmds;
	t_token	*cur;
	t_token	*cmd_starts[256];
	int		heredoc_fds[256];
	int		i;
	int		j;
	int		pipefd[256][2];
	pid_t	pids[256];
	int		status;
	int		cmd_argc;
	char	*cmd_argv[MAX_ARGS];
	t_token	*seg;
	int		k;
	t_token	*temp;
	t_token	*cmd_token;
	int		sig;

	i = 0;
	num_cmds = 0;
	cur = token;
	k = 0;
	while (k < 256)
	{
		heredoc_fds[k] = -1;
		k++;
	}
	while (cur)
	{
		if (i == 0 || (cur && ft_strcmp(cur->type, "pipe") == 0 && cur->next))
		{
			if (i == 0)
				cmd_starts[num_cmds] = cur;
			else
				cmd_starts[num_cmds] = cur->next;
			temp = cmd_starts[num_cmds];
			while (temp && ft_strcmp(temp->type, "pipe") != 0)
			{
				if (ft_strcmp(temp->type, "here-document") == 0)
				{
					heredoc_fds[num_cmds] = setup_redirection(temp);
					if (heredoc_fds[num_cmds] == -1)
					{
						perror("heredoc failed");
						set_shell_status(1);
						return ;
					}
				}
				temp = temp->next;
			}
			num_cmds++;
		}
		cur = cur->next;
		i++;
	}
	if (num_cmds == 1)
	{
		seg = cmd_starts[0];
		cmd_argc = 0;
		while (seg && ft_strcmp(seg->type, "pipe") != 0)
		{
			if (ft_strncmp(seg->type, "redirect", 8) == 0
				|| ft_strcmp(seg->type, "here-document") == 0
				|| ft_strcmp(seg->type, "append output") == 0)
			{
				seg = seg->next;
				continue ;
			}
			cmd_argv[cmd_argc++] = seg->value;
			seg = seg->next;
		}
		cmd_argv[cmd_argc] = NULL;
		if (is_shell_builtin(cmd_argv[0]) && should_run_in_parent(cmd_argv[0]))
		{
			if (!ft_strcmp(cmd_argv[0], "export"))
				handle_export_command(envp, cmd_argv, cmd_argc);
			else if (!ft_strcmp(cmd_argv[0], "unset"))
				handle_unset_command(envp, cmd_argv, cmd_argc);
			else if (!ft_strcmp(cmd_argv[0], "cd"))
				handle_cd_command(cmd_argv[1], cmd_argc, envp);
			else if (!ft_strcmp(cmd_argv[0], "exit"))
				handle_exit_command(cmd_argv, cmd_argc, *envp);
			if (heredoc_fds[0] != -1)
				close(heredoc_fds[0]);
			return ;
		}
	}
	ignore_signals();
	i = 0;
	while (i < num_cmds - 1)
	{
		if (pipe(pipefd[i]) < 0)
		{
			perror("pipe");
			set_shell_status(1);
			restore_signals();
			return ;
		}
		i++;
	}
	i = 0;
	while (i < num_cmds)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			perror("fork");
			set_shell_status(1);
			restore_signals();
			return ;
		}
		if (pids[i] == 0)
		{
			setup_child_signals();
			if (i > 0)
			{
				dup2(pipefd[i - 1][0], STDIN_FILENO);
			}
			if (i < num_cmds - 1)
			{
				dup2(pipefd[i][1], STDOUT_FILENO);
			}
			j = 0;
			while (j < num_cmds - 1)
			{
				close(pipefd[j][0]);
				close(pipefd[j][1]);
				j++;
			}
			if (heredoc_fds[i] != -1)
			{
				dup2(heredoc_fds[i], STDIN_FILENO);
				close(heredoc_fds[i]);
			}
			seg = cmd_starts[i];
			cmd_argc = 0;
			while (seg && ft_strcmp(seg->type, "pipe") != 0)
			{
				if (ft_strncmp(seg->type, "redirect", 8) == 0
					|| ft_strcmp(seg->type, "append output") == 0)
				{
					if (setup_redirection(seg) == -1)
						exit(1);
					seg = seg->next;
					continue ;
				}
				else if (ft_strcmp(seg->type, "here-document") == 0)
				{
					// skip heredoc tokens in argv
					seg = seg->next;
					continue ;
				}
				cmd_argv[cmd_argc++] = seg->value;
				seg = seg->next;
			}
			cmd_argv[cmd_argc] = NULL;
			if (cmd_argc == 0 || cmd_argv[0] == NULL)
				exit(0);
			if (is_shell_builtin(cmd_argv[0]))
			{
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
				else if (!ft_strcmp(cmd_argv[0], "env"))
					handle_env_command(*envp);
				else if (!ft_strcmp(cmd_argv[0], "pwd"))
					handle_pwd_command(envp);
				exit(0);
			}
			execvp(cmd_argv[0], cmd_argv);
			perror(cmd_argv[0]);
			exit(127);
		}
		i++;
	}
	i = 0;
	while (i < num_cmds - 1)
	{
		close(pipefd[i][0]);
		close(pipefd[i][1]);
		i++;
	}
	i = 0;
	while (i < num_cmds)
	{
		if (heredoc_fds[i] != -1)
			close(heredoc_fds[i]);
		i++;
	}
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
	restore_signals();
}

char	*get_input(void)
{
	char	*line;
	char	*new_part;
	char	*combined;
	char	cwd[1024];
	char	*prompt;
	char	*tmp;

	while (1)
	{
		if (getcwd(cwd, sizeof(cwd)))
		{
			tmp = ft_strdup(cwd);
			prompt = ft_strjoin(tmp, "$ ");
			free(tmp);
		}
		else
			prompt = ft_strdup("$ ");
		line = readline(prompt);
		free(prompt);
		if (!line)
		{
			printf("exit\n");
			return (NULL);
		}
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		if (!has_unclosed_quotes(line))
		{
			add_history(line);
			return (line);
		}
		while (has_unclosed_quotes(line))
		{
			new_part = readline("> ");
			if (!new_part)
			{
				free(line);
				return (NULL);
			}
			combined = ft_strjoin(line, "\n");
			free(line);
			line = ft_strjoin(combined, new_part);
			free(combined);
			free(new_part);
		}
		add_history(line);
		return (line);
	}
}
