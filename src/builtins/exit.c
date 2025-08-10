/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 12:00:00 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/10 14:38:46 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_valid_number(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	check_overflow(const char *str)
{
	long long	num;

	num = ft_atol(str);
	if (num > LLONG_MAX || num < LLONG_MIN)
		return (1);
	return (0);
}

int	process_exit_args(char **args, int arg_count)
{
	if (arg_count == 1)
		return (get_shell_status());
	if (arg_count == 2)
	{
		if (!is_valid_number(args[1]) || check_overflow(args[1]))
			return (-2);
		return ((unsigned char)ft_atol(args[1]));
	}
	return (-1);
}

void	handle_exit_command(char **args, int arg_count, char **envp)
{
	int	exit_code;

	(void)envp;
	printf("\033[0;31mexit\n\033[0m");
	exit_code = process_exit_args(args, arg_count);
	if (exit_code == -1)
	{
		perror("minishell: exit: too many arguments");
		set_shell_status(1);
		return ;
	}
	else if (exit_code == -2)
	{
		perror("minishell: exit: numeric argument required");
		ft_free_arr((void ***)&envp);
		rl_clear_history();
		exit(2);
	}
	else
	{
		ft_free_arr((void ***)&envp);
		rl_clear_history();
		exit(exit_code);
	}
}
