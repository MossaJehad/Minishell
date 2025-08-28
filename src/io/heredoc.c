/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:32:55 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/28 16:31:09 by mhasoneh         ###   ########.fr       */
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
		if (g_signal == 130)
		{
			free(line);
			return (-1);
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

int	handle_heredoc(t_token *tok, t_exec_ctx *ctx)
{
	int	pipefd[2];

	(void)ctx;
	if (create_heredoc_pipe(pipefd) == -1)
		return (-1);
	if (read_heredoc_lines(pipefd[1], tok->value) == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}
