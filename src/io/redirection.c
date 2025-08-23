/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:32:55 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/23 19:05:03 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_file_redirection(t_token *tok)
{
	int	fd;

	if (!tok->next || !tok->next->value)
	{
		printf("syntax error: missing filename\n");
		return (-1);
	}
	if (tok->type == REDIRECT)
		fd = open(tok->next->value, O_RDONLY);
	else if (tok->type == REDIRECT_OUT)
		fd = open(tok->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (tok->type == APPEND)
		fd = open(tok->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (-1);
	if (fd < 0)
	{
		perror(tok->next->value);
		return (-1);
	}
	return (fd);
}

int	setup_redirection(t_token *tok)
{
	int	fd;

	if (!tok)
		return (-1);
	if (tok->type == HEREDOC)
	{
		if (!tok->next || !tok->next->value)
		{
			printf("syntax error: missing heredoc delimiter\n");
			return (-1);
		}
		return (handle_heredoc(tok->next, (t_exec_ctx){0}));
	}
	fd = handle_file_redirection(tok);
	if (fd < 0)
		return (-1);
	if (tok->type == REDIRECT)
		dup2(fd, STDIN_FILENO);
	else if (tok->type == REDIRECT_OUT || tok->type == APPEND)
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (fd);
}
