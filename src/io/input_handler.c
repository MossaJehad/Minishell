/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 13:11:53 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/29 12:06:48 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*append_until_quotes_closed(char *line)
{
	if (has_unclosed_quotes(line))
	{
		printf("Syntax error \n");
		free(line);
		return (NULL);
	}
	return (line);
}
