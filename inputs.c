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
				free(line);
				break ;
			}
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
			free(line);
		}
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		return (0);
	}
	if (ft_strcmp(tok->type, "redirect input") == 0)
		fd = open(tok->value, O_RDONLY);
	else if (ft_strcmp(tok->type, "redirect output") == 0)
		fd = open(tok->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(tok->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
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
	int		i = 0, j;
	int		pipefd[256][2];
	pid_t	pids[256];
	int		status;
	int		cmd_argc;
	char	*cmd_argv[MAX_ARGS];
	t_token	*seg;

	(void)input; // Mark unused parameter as intentionally unused
	(void)args;  // Mark unused parameter as intentionally unused
	(void)arg_count; // Mark unused parameter as intentionally unused
	(void)envp;  // Mark unused parameter as intentionally unused

	// 1. Find all command start tokens (split by pipe)
	while (cur) {
		if (i == 0 || (cur && ft_strcmp(cur->type, "pipe") == 0 && cur->next)) {
			cmd_starts[num_cmds++] = (i == 0) ? cur : cur->next;
		}
		cur = cur->next;
		i++;
	}

	// 2. Create pipes for all but the last command
	for (i = 0; i < num_cmds - 1; i++) {
		if (pipe(pipefd[i]) < 0) {
			perror("pipe");
			return;
		}
	}

	// 3. Fork for each command segment
	for (i = 0; i < num_cmds; i++) {
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
			for (j = 0; j < num_cmds - 1; j++) {
				close(pipefd[j][0]);
				close(pipefd[j][1]);
			}
			// d. Collect arguments for this command
			seg = cmd_starts[i];
			cmd_argc = 0;
			while (seg && ft_strcmp(seg->type, "pipe") != 0) {
				if (ft_strncmp(seg->type, "redirect", 8) == 0 || ft_strcmp(seg->type, "here-document") == 0) {
					if (setup_redirection(seg) == -1)
						exit(1);
					seg = seg->next;
					continue;
				}
				cmd_argv[cmd_argc++] = seg->value;
				seg = seg->next;
			}
			cmd_argv[cmd_argc] = NULL;
			// e. Execute command
			execvp(cmd_argv[0], cmd_argv);
			perror(cmd_argv[0]);
			exit(1);
		}
	}

	// 4. Parent closes all pipe fds
	for (i = 0; i < num_cmds - 1; i++) {
		close(pipefd[i][0]);
		close(pipefd[i][1]);
	}

	// 5. Wait for all children
	for (i = 0; i < num_cmds; i++) {
		waitpid(pids[i], &status, 0);
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

	line = NULL;
	new_part = NULL;
	combined = NULL;

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
// }
// 			return (line);
// 		}
// 		while (has_unclosed_quotes(line))
// 		{
// 			new_part = readline("> ");
// 			if (!new_part)
// 			{
// 				free(line);
// 				return (NULL);
// 			}
// 			combined = ft_strjoin(line, "\n");
// 			free(line);
// 			line = ft_strjoin(combined, new_part);
// 			free(combined);
// 			free(new_part);
// 		}
// 		add_history(line);
// 		return (line);
// 	}
// }
