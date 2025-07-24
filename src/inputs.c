/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zal-qais <zal-qais@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:18 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/07/21 20:25:27 by zal-qais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	setup_redirection(t_token *tok)
{
	int pipefd[2];
	char *line;
	int	fd;

	if (ft_strcmp(tok->type, "here-document") == 0)
	{
		if (pipe(pipefd) == -1)
			return (perror("pipe"), -1);
		while (1)
		{
			line = readline("> ");
			if (!line || ft_strcmp(line, tok->value) == 0)
			{
				if (line)
					free(line);
				break ;
			}
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
			free(line);
		}
		close(pipefd[1]);
		return (pipefd[0]); // Return read end for later use
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

void	handle_command(char *input, char **args, int arg_count, t_token *token,
		char ***envp)
{
	int		num_cmds = 0;
	t_token	*cur = token;
	t_token	*cmd_starts[256];
	int		heredoc_fds[256];
	int		i = 0, j;
	int		pipefd[256][2];
	pid_t	pids[256];
	int		status;
	int		cmd_argc;
	char	*cmd_argv[MAX_ARGS];
	t_token	*seg;
	int		k;

	(void)input;
	(void)args;
	(void)arg_count;

	// Initialize heredoc_fds
	k = 0;
	while (k < 256)
	{
		heredoc_fds[k] = -1;
		k++;
	}

	// 1. Find all command start tokens and process heredocs in order
	while (cur) {
		if (i == 0 || (cur && ft_strcmp(cur->type, "pipe") == 0 && cur->next)) {
			cmd_starts[num_cmds] = (i == 0) ? cur : cur->next;
			
			// Process heredocs for this command
			t_token *temp = cmd_starts[num_cmds];
			while (temp && ft_strcmp(temp->type, "pipe") != 0) {
				if (ft_strcmp(temp->type, "here-document") == 0) {
					heredoc_fds[num_cmds] = setup_redirection(temp);
					if (heredoc_fds[num_cmds] == -1) {
						perror("heredoc failed");
						return;
					}
				}
				temp = temp->next;
			}
			num_cmds++;
		}
		cur = cur->next;
		i++;
	}

	// Special case: single built-in command without pipes
	if (num_cmds == 1) {
		seg = cmd_starts[0];
		cmd_argc = 0;
		
		// Collect arguments for this command
		while (seg && ft_strcmp(seg->type, "pipe") != 0) {
			if (ft_strncmp(seg->type, "redirect", 8) == 0 || 
				ft_strcmp(seg->type, "here-document") == 0 ||
				ft_strcmp(seg->type, "append output") == 0) {
				seg = seg->next;
				continue;
			}
			cmd_argv[cmd_argc++] = seg->value;
			seg = seg->next;
		}
		cmd_argv[cmd_argc] = NULL;

		// Check if it's a built-in that should run in parent
		if (is_shell_builtin(cmd_argv[0]) && should_run_in_parent(cmd_argv[0])) {
			if (!ft_strcmp(cmd_argv[0], "export"))
				handle_export_command(envp, cmd_argv, cmd_argc);
			else if (!ft_strcmp(cmd_argv[0], "unset"))
				handle_unset_command(envp, cmd_argv, cmd_argc);
			else if (!ft_strcmp(cmd_argv[0], "cd"))
				handle_cd_command(cmd_argv[1], cmd_argc);
			else if (!ft_strcmp(cmd_argv[0], "exit"))
				exit(0);
			// Clean up heredoc fd
			if (heredoc_fds[0] != -1)
				close(heredoc_fds[0]);
			return;
		}
	}

	// 2. Create pipes for all but the last command
	i = 0;
	while (i < num_cmds - 1)
	{
		if (pipe(pipefd[i]) < 0) {
			perror("pipe");
			return;
		}
		i++;
	}

	// 3. Fork for each command segment
	i = 0;
	while (i < num_cmds)
	{
		pids[i] = fork();
		if (pids[i] < 0) {
			perror("fork");
			return;
		}
		if (pids[i] == 0) {
			// Child process
			// a. Set up input from previous pipe if not first command
			if (i > 0) {
				dup2(pipefd[i-1][0], STDIN_FILENO);
			}
			// b. Set up output to next pipe if not last command
			if (i < num_cmds - 1) {
				dup2(pipefd[i][1], STDOUT_FILENO);
			}
			// c. Close all pipe fds in child
			j = 0;
			while (j < num_cmds - 1)
			{
				close(pipefd[j][0]);
				close(pipefd[j][1]);
				j++;
			}
			
			// d. Use heredoc input if available
			if (heredoc_fds[i] != -1) {
				dup2(heredoc_fds[i], STDIN_FILENO);
				close(heredoc_fds[i]);
			}
			
			// e. Collect arguments and handle other redirections for this command
			seg = cmd_starts[i];
			cmd_argc = 0;
			while (seg && ft_strcmp(seg->type, "pipe") != 0) {
				if (ft_strncmp(seg->type, "redirect", 8) == 0 || 
					ft_strcmp(seg->type, "append output") == 0) {
					if (setup_redirection(seg) == -1)
						exit(1);
					seg = seg->next;
					continue;
				}
				else if (ft_strcmp(seg->type, "here-document") == 0) {
					// Skip heredoc tokens as they're already processed
					seg = seg->next;
					continue;
				}
				cmd_argv[cmd_argc++] = seg->value;
				seg = seg->next;
			}
			cmd_argv[cmd_argc] = NULL;
			
			// f. Handle built-in commands in child process
			if (is_shell_builtin(cmd_argv[0])) {
				if (!ft_strcmp(cmd_argv[0], "echo")) {
					// Create a temporary token list for this command segment
					t_token *cmd_token = NULL;
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
					printf("%s\n", getenv("PWD") ? getenv("PWD") : "");
				exit(0);
			}
			
			// g. Execute external command
			execvp(cmd_argv[0], cmd_argv);
			perror(cmd_argv[0]);
			exit(1);
		}
		i++;
	}

	// 4. Parent closes all pipe fds and heredoc fds
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

	// 5. Wait for all children
	i = 0;
	while (i < num_cmds)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}
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
		// Generate prompt
		if (getcwd(cwd, sizeof(cwd)))
		{
			tmp = ft_strdup(cwd);
			prompt = ft_strjoin(tmp, "$ ");
			free(tmp);
		}
		else
			prompt = ft_strdup("$ ");

		// Read input
		line = readline(prompt);
		free(prompt);

		// Handle EOF
		if (!line)
		{
			printf("exit\n");
			return (NULL);
		}

		// Handle empty input
		if (*line == '\0')
		{
			free(line);
			continue;
		}

		// Check for unclosed quotes
		if (!has_unclosed_quotes(line))
		{
			add_history(line);
			return (line);
		}

		// Handle unclosed quotes
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
