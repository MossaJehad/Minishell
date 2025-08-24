/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:32:55 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/24 17:59:45 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	create_heredoc_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	return (0);
}

int	handle_heredoc_child(int pipefd[2], t_token *tok, t_exec_ctx ctx)
{
	setup_heredoc_signals();
	close(pipefd[0]);
	if (read_heredoc_lines(pipefd[1], tok->value) == -1)
	{
		close(pipefd[1]);
		close_heredoc_fds(ctx.heredoc_fds, ctx.cmd_count);
		exit(1);
	}
	close(pipefd[1]);
	close_heredoc_fds(ctx.heredoc_fds, ctx.cmd_count);
	exit(0);
}

int	handle_heredoc_parent(pid_t pid, int pipefd[2], int *status)
{
	close(pipefd[1]);
	waitpid(pid, status, 0);
	if (WIFSIGNALED(*status) && WTERMSIG(*status) == SIGINT)
	{
		close(pipefd[0]);
		g_signal = 130;
		return (-1);
	}
	return (pipefd[0]);
}

/*
	HEREDOC SHOULD RUN IN PARENT NOT CHILD
	- Uses pipes, read from the strin, <, <<
*/
int	handle_heredoc(t_token *tok, t_exec_ctx ctx)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (create_heredoc_pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
		handle_heredoc_child(pipefd, tok, ctx);
	return (handle_heredoc_parent(pid, pipefd, &status));
}

int	read_heredoc_lines(int write_fd, const char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("\n");
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (write(write_fd, line, ft_strlen(line)) == -1 || write(write_fd,
				"\n", 1) == -1)
		{
			perror("write to heredoc pipe");
			free(line);
			return (-1);
		}
		free(line);
	}
	return (0);
}
