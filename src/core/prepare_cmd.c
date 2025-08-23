/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:53:36 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/23 19:54:09 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*handle_operator(char **args, char **envp)
{
	t_token	*token;

	args = expand(args, envp);
	if (!args)
		return (NULL);
	if (check_syntax_error(args))
	{
		ft_free_arr(args);
		set_shell_status(2);
		return (NULL);
	}
	token = NULL;
	tokenize(args, &token);
	ft_free_arr(args);
	if (!token)
	{
		set_shell_status(1);
		ft_free_arr(args);
		return (NULL);
	}
	return (token);
}

char	**parse_and_validate(char *input, int *arg_count)
{
	char	**args;

	args = parse_arguments(input, arg_count);
	if (!args)
		return (NULL);
	if (*arg_count == 0 || !args[0])
	{
		ft_free_arr(args);
		return (NULL);
	}
	return (args);
}

char	**expand_and_validate(char **args, char **envp)
{
	args = expand(args, envp);
	if (!args)
		return (NULL);
	if (!args[0] || args[0][0] == '\0')
	{
		ft_free_arr(args);
		set_shell_status(0);
		return (NULL);
	}
	if (args[0][0] == '$' && !command_exists(args[0] + 1, envp))
	{
		printf("%s: command not found\n", args[0]);
		ft_free_arr(args);
		set_shell_status(127);
		return (NULL);
	}
	if (!command_exists(args[0], envp))
	{
		printf("%s: command not found\n", args[0]);
		ft_free_arr(args);
		set_shell_status(127);
		return (NULL);
	}
	return (args);
}

t_token	*prepare_command(char *input, char ***envp, int arg_count)
{
	char	**args;
	t_token	*token;

	token = NULL;
	args = parse_and_validate(input, &arg_count);
	if (!args)
		return (NULL);
	if (is_operator(args[0]))
		return (handle_operator(args, *envp));
	args = expand_and_validate(args, *envp);
	if (!args)
		return (NULL);
	if (check_syntax_error(args))
	{
		ft_free_arr(args);
		set_shell_status(2);
		return (NULL);
	}
	tokenize(args, &token);
	ft_free_arr(args);
	return (token);
}
