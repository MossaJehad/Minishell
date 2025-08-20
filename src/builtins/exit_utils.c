/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 09:18:58 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/20 02:44:07 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/minishell.h"

int	test_max_min(char *arg)
{
	long	max;
	long	min;

	if (!arg)
		return (1);
	if (!ft_atol(arg))
		return (0);
	if (arg[0] == '-')
	{
		if (ft_strlen(arg) > 20)
			return (1);
		min = ft_atol(arg);
		if (min >= 0 && min <= LLONG_MAX)
			return (1);
	}
	else
	{
		if (ft_strlen(arg) > 19)
			return (1);
		max = ft_atol(arg);
		if (max <= 0 && max >= LLONG_MIN)
			return (1);
	}
	return (0);
}

int	is_exit_correct(t_shell *shell, char *arg, int i)
{
	long	ret;
	int		sign;

	ret = 0;
	sign = 0;
	if (test_max_min(arg))
		return (1);
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	if (arg[0] == '-')
		sign = -1;
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (1);
		ret = (ret * 10) + arg[i] - 48;
		if (ret > 255)
			ret %= 256;
		i++;
	}
	if (sign == -1 && ret)
		ret = 256 - ret;
	shell->last_status = ret;
	return (0);
}

void	exit_err(t_shell *shell)
{
	int	n;
	int	code;

	n = count_line(shell->cmd->full_cmd);
	code = 0;
	if (is_exit_correct(shell, shell->cmd->full_cmd[1], 0))
	{
		shell->last_status = 2;
		err_message(shell, shell->cmd->full_cmd[0], shell->cmd->full_cmd[1], \
		"numeric argument required");
	}
	else if (n > 2)
	{
		code = 1;
		shell->last_status = code;
		err_message2(shell->cmd->full_cmd[0], NULL, "too many arguments");
		return ;
	}
	cleanup_all(shell);
	free(shell->input);
	ft_free_arr(shell->envp);
	free(shell);
	exit(code);
}

void	simple_exit2(t_shell *shell, int code)
{
	perror("");
	code = shell->last_status;
	cleanup_all(shell);
	ft_free_arr(shell->envp);
	free(shell);
	exit(code);
}
