/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malja-fa <malja-fa@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 13:11:53 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/30 11:16:26 by malja-fa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*append_until_quotes_closed(char *line)
{
	if (has_unclosed_quotes(line))
	{
		printf("Syntax error \n");
		set_shell_status(1);
		free(line);
		return (NULL);
	}
	return (line);
}
