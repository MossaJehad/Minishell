/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 12:53:23 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/30 15:55:46 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_operator(const char *s)
{
	if (!s)
		return (0);
	return (ft_strcmp(s, "|") == 0 || ft_strcmp(s, "<") == 0 || ft_strcmp(s,
			">") == 0 || ft_strcmp(s, ">>") == 0 || ft_strcmp(s, "<<") == 0);
}

int	should_run_in_parent(const char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "exit"));
}

void	cleanup_shell_resources(char ***env, t_token *token, char **args,
		char *input)
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

void	handle_exit_argument(char *arg)
{
	int	exit_code;

	if (!is_valid_number(arg) || check_overflow(arg))
	{
		printf("minishell: exit: %s: numeric argument required\n", arg);
		exit(2);
	}
	exit_code = (unsigned char)ft_atol(arg);
	exit(exit_code);
}

char	*extract_inner_single_quote(const char *arg, int len)
{
	char	*str;
	int		i;
	int		j;

	str = ft_calloc(len - 1, sizeof(char));
	if (!str)
		return (NULL);
	i = 1;
	j = 0;
	while (i < len - 1)
		str[j++] = arg[i++];
	str[j] = '\0';
	return (str);
}
