/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 12:00:00 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/07/31 15:01:34 by mhasoneh         ###   ########.fr       */
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

long long	ft_atoll(const char *str)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

int	check_overflow(const char *str)
{
	long long	num;

	num = ft_atoll(str);
	if (num > LLONG_MAX || num < LLONG_MIN)
		return (1);
	return (0);
}

void	handle_exit_command(char **args, int arg_count, char **envp)
{
	int	exit_code;

	printf("exit\n");
	if (arg_count == 1)
	{
		free_env(envp);
		rl_clear_history();
		exit(get_exit_status());
	}
	else if (arg_count == 2)
	{
		if (!is_valid_number(args[1]))
		{
			printf("minishell: exit: %s: numeric argument required\n", args[1]);
			free_env(envp);
			rl_clear_history();
			exit(2);
		}
		if (check_overflow(args[1]))
		{
			printf("minishell: exit: %s: numeric argument required\n", args[1]);
			free_env(envp);
			rl_clear_history();
			exit(2);
		}
		exit_code = (unsigned char)ft_atoll(args[1]);
		free_env(envp);
		rl_clear_history();
		exit(exit_code);
	}
	else
	{
		printf("minishell: exit: too many arguments\n");
		set_exit_status(1);
		return ;
	}
}

void	cleanup_and_exit(int exit_code, char **envp)
{
	free_env(envp);
	rl_clear_history();
	exit(exit_code);
}
