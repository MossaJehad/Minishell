/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 12:00:00 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 12:23:12 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

// Helper: process exit arguments, return:
//  -1 if too many arguments (error)
//  -2 if invalid numeric argument (error)
//  >=0 valid exit code
static int process_exit_args(char **args, int arg_count)
{
	if (arg_count == 1)
		return get_shell_status();
	if (arg_count == 2)
	{
		if (!is_valid_number(args[1]) || check_overflow(args[1]))
			return -2;
		return (unsigned char)ft_atol(args[1]);
	}
	return -1;
}

void handle_exit_command(char **args, int arg_count, char **envp)
{
	int	exit_code;

	printf("exit\n");
	exit_code = process_exit_args(args, arg_count);
	if (exit_code == -1)
	{
		perror("minishell: exit: too many arguments");
		set_shell_status(1);
		return;
	}
	else if (exit_code == -2)
	{
		perror("minishell: exit: numeric argument required");
		free_env(envp);
		rl_clear_history();
		exit(2);
	}
	else
	{
		free_env(envp);
		rl_clear_history();
		exit(exit_code);
	}
}

void	cleanup_and_exit(int exit_code, char **envp)
{
	free_env(envp);
	rl_clear_history();
	exit(exit_code);
}
