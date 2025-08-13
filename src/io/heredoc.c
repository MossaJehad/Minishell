/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:32:55 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 05:11:13 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_heredoc(const char *delimiter)
{
	int		pipe_fds[2];
	char	*line;
	pid_t	pid;

	if (pipe(pipe_fds) == -1)
	{
		perror("pipe");
		return (-1);
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		return (-1);
	}

	if (pid == 0)
	{
		/* Child process - read heredoc input */
		close(pipe_fds[0]);
		setup_child_signals();

		while (1)
		{
			line = readline("> ");
			if (!line || ft_strcmp(line, delimiter) == 0)
			{
				free(line);
				break;
			}

			write(pipe_fds[1], line, ft_strlen(line));
			write(pipe_fds[1], "\n", 1);
			free(line);
		}

		close(pipe_fds[1]);
		exit(EXIT_SUCCESS);
	}
	else
	{
		/* Parent process */
		close(pipe_fds[1]);
		waitpid(pid, NULL, 0);
		return (pipe_fds[0]);
	}
}

//int	read_heredoc_lines(int write_fd, const char *delimiter)
//{
//	char	*line;

//	while (1)
//	{
//		line = readline("> ");
//		if (!line)
//		{
//			printf("\n");
//			break ;
//		}
//		if (ft_strcmp(line, delimiter) == 0)
//		{
//			free(line);
//			break ;
//		}
//		if (write(write_fd, line, ft_strlen(line)) == -1 || write(write_fd,
//				"\n", 1) == -1)
//		{
//			perror("write to heredoc pipe");
//			free(line);
//			return (-1);
//		}
//		free(line);
//	}
//	return (0);
//}
