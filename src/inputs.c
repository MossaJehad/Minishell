/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:18 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/07/23 14:25:25 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


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
	int		saved_in;
	int		saved_out;
	char	*exec_args[MAX_ARGS];
	int		ei;
	t_token	*cur;
	pid_t	pid;
	int		i;
	int		j;
			char cwd[1024];

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	ei = 0;
	cur = token;
	i = 0;
	while (cur)
		cur = cur->next;
	while (i < arg_count)
	{
		if (!ft_strcmp(args[i], "<") || !ft_strcmp(args[i], ">")
			|| !ft_strcmp(args[i], ">>") || !ft_strcmp(args[i], "<<"))
		{
			i += 2;
			continue ;
		}
		exec_args[ei++] = args[i++];
	}
	exec_args[ei] = NULL;
	// ---- parent‐run builtins ----
	if (exec_args[0] && should_run_in_parent(exec_args[0]))
	{
		if (!ft_strcmp(exec_args[0], "cd"))
			handle_cd_command(exec_args[1], ei);
		else if (!ft_strcmp(exec_args[0], "export"))
			handle_export_command(envp, exec_args, ei);
		else if (!ft_strcmp(exec_args[0], "unset"))
			handle_unset_command(envp, exec_args, ei);
		else if (!ft_strcmp(exec_args[0], "exit"))
		{
			/* restore and close before exiting */
			dup2(saved_in, STDIN_FILENO);
			dup2(saved_out, STDOUT_FILENO);
			close(saved_in);
			close(saved_out);
			exit(0);
		}
		dup2(saved_in, STDIN_FILENO);
		dup2(saved_out, STDOUT_FILENO);
		close(saved_in);
		close(saved_out);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		cur = token;
		while (cur)
		{
			if (ft_strcmp(cur->type, "redirect input") == 0
				|| ft_strcmp(cur->type, "redirect output") == 0
				|| ft_strcmp(cur->type, "append output") == 0
				|| ft_strcmp(cur->type, "here-document") == 0)
			{
				if (setup_redirection(cur) == -1)
					exit(1);
			}
			cur = cur->next;
		}
		if (exec_args[0] && ft_strcmp(exec_args[0], "echo") == 0)
			handle_echo_command(token);
		else if (exec_args[0] && ft_strcmp(exec_args[0], "env") == 0)
			handle_env_command(*envp);
		else if (exec_args[0] && ft_strcmp(exec_args[0], "export") == 0)
			handle_export_command(envp, exec_args, ei);
		else if (exec_args[0] && ft_strcmp(exec_args[0], "type") == 0)
		{
			j = 0;
			while (j < ei)
				handle_type_command(exec_args[j++]);
		}
		else if (exec_args[0] && ft_strcmp(exec_args[0], "pwd") == 0)
		{
			printf("%s\n", getcwd(cwd, sizeof(cwd)));
		}
		else if (exec_args[0] && ft_strcmp(exec_args[0], "cd") == 0)
			handle_cd_command(input + 2, ei);
		else if (exec_args[0] && ft_strcmp(exec_args[0], "exit") == 0 && ei > 1
			&& ft_strcmp(exec_args[1], "0") == 0)
			exit(0);
		else if (exec_args[0] && ft_strcmp(exec_args[0], "unset") == 0)
			handle_unset_command(envp, exec_args, ei);
		else
			execvp(exec_args[0], exec_args), perror(exec_args[0]);
		exit(0);
	}
	else if (pid > 0)
	{
		waitpid(pid, NULL, 0);
		dup2(saved_in, STDIN_FILENO);
		dup2(saved_out, STDOUT_FILENO);
		close(saved_in);
		close(saved_out);
	}
	else
		perror("fork");
}

char	*get_input(void)
{
	char	*line;
	char	*new_part;
	char	*combined;
			char cwd[1024];
			char *prompt;
	char	*tmp;

	line = NULL;
	new_part = NULL;
	combined = NULL;
	while (1)
	{
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
		}
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
