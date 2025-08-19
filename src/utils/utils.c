/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 12:53:23 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/19 03:29:12 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_operator(const char *s)
{
	if (!s)
		return (0);
	return (
		ft_strcmp(s, "|")	== 0 ||
		ft_strcmp(s, "<")	== 0 ||
		ft_strcmp(s, ">")	== 0 ||
		ft_strcmp(s, ">>")	== 0 ||
		ft_strcmp(s, "<<")	== 0
	);
}

int	should_run_in_parent(const char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "exit"));
}

void	cleanup_and_exit(int exit_code)
{
	rl_clear_history();
	exit(exit_code);
}

void	cleanup_shell_resources(char ***env, t_token *token, char **args, char *input)
{
	if (token)
	{
		free_tokens(token);
		token = NULL;
	}
	if (args)
	{
		ft_free_arr(args);
		args = NULL;
	}
	if (input)
	{
		free(input);
		input = NULL;
	}
	if (env && *env)
	{
		ft_free_arr(*env);
		*env = NULL;
	}
}
