/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:53:11 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/30 15:54:55 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	validate_exit_arguments(char *arg)
{
	if (!is_valid_number(arg) || check_overflow(arg))
	{
		printf("minishell: exit: %s: numeric argument required\n", arg);
		return (2);
	}
	else
	{
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
}

int	handle_exit_code(char *cmd_argv[MAX_ARGS], int cmd_argc)
{
	int	exit_code;

	exit_code = 0;
	if (cmd_argc == 1)
		exit_code = get_shell_status();
	else if (cmd_argc == 2)
	{
		if (!is_valid_number(cmd_argv[1]) || check_overflow(cmd_argv[1]))
		{
			printf("minishell: exit: %s: numeric argument required\n",
				cmd_argv[1]);
			return (2);
		}
		exit_code = (unsigned char)ft_atol(cmd_argv[1]);
	}
	else
		return (validate_exit_arguments(cmd_argv[1]));
	return (exit_code);
}

void	exit_with_cleanup(int exit_code, char **envp, t_exec_ctx *ctx)
{
	cleanup_shell_resources(&envp, *ctx->cmd_starts, NULL, NULL);
	exit(exit_code);
}
