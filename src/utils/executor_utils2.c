/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 12:59:22 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/30 13:00:41 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_dir(char *cmd, int flag)
{
	struct stat	sb;

	if (!stat(cmd, &sb) && S_ISDIR(sb.st_mode))
	{
		if (ft_strcmp(cmd, ".") == 0 || ft_strcmp(cmd, "..") == 0)
			return (127);
		else
		{
			if (flag)
			{
				write(2, cmd, ft_strlen(cmd));
				write(2, ": Is a directory\n", 18);
			}
			return (126);
		}
	}
	return (0);
}
