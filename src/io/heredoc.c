/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:32:55 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/30 13:01:06 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	create_heredoc_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	return (0);
}

int	handle_write_line(int write_fd, char *line)
{
	if (write(write_fd, line, ft_strlen(line)) == -1
		|| write(write_fd, "\n", 1) == -1)
	{
		perror("write to heredoc pipe");
		free(line);
		return (-1);
	}
	free(line);
	return (0);
}

int	process_line(int write_fd, char *line, const char *delimiter)
{
	if (!line)
	{
		printf("\n");
		return (1);
	}
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (1);
	}
	return (handle_write_line(write_fd, line));
}

int	read_heredoc_lines(int write_fd, const char *delimiter)
{
	char	*line;
	int		status;

	signal(SIGINT, sig_heredoc);
	while (1)
	{
		line = readline("> ");
		if (g_signal == SIGINT)
			return (handle_sigint_case(line));
		status = process_line(write_fd, line, delimiter);
		if (status != 0)
		{
			if (status == -1)
				return (-1);
			else
				return (0);
		}
	}
}

int	handle_heredoc(t_token *tok, t_exec_ctx *ctx)
{
	int	pipefd[2];
	int	result;

	(void)ctx;
	if (create_heredoc_pipe(pipefd) == -1)
		return (-1);
	result = read_heredoc_lines(pipefd[1], tok->value);
	close(pipefd[1]);
	if (result == -1)
	{
		close(pipefd[0]);
		return (-1);
	}
	return (pipefd[0]);
}
