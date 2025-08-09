/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:32:55 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 17:35:18 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int setup_redirection(t_token *tok)
{
	int fd;

	if (validate_token(tok) < 0)
		return (-1);
	if (ft_strcmp(tok->type, "here-document") == 0)
		return (handle_heredoc(tok));
	fd = handle_file_redirection(tok);
	if (fd < 0)
		return (-1);
	if (ft_strcmp(tok->type, "redirect input") == 0)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int handle_file_redirection(t_token *tok)
{
	int	fd;

	fd = -1;
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
	return (fd);
}
