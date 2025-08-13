/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:51:06 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 05:31:28 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_unset(t_shell *shell, char **args)
{
	int	i;

	if (!args[1])
		return (EXIT_SUCCESS);

	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			print_error("unset", args[i], "not a valid identifier");
			return (EXIT_FAILURE);
		}

		env_remove(shell, args[i]);
		i++;
	}

	return (EXIT_SUCCESS);
}

void	handle_unset_command(char ***envp, char **args, int arg_count)
{
	int	i;

	i = 1;
	while (i < arg_count)
	{
		if (!is_valid_identifier(args[i]))
			perror("unset: not a valid identifier");
		else
			remove_env_var(envp, args[i]);
		i++;
	}
}

void	remove_env_var(char ***envp, const char *name)
{
	char	**env;
	int		i;
	int		cnt;

	env = *envp;
	cnt = 0;
	while (env[cnt])
		cnt++;
	i = 0;
	while (i < cnt)
	{
		if (ft_strncmp(env[i], name, ft_strlen(name)) == 0
			&& env[i][ft_strlen(name)] == '=')
		{
			free(env[i]);
			while (i < cnt - 1)
			{
				env[i] = env[i + 1];
				i++;
			}
			env[i] = NULL;
			break ;
		}
		i++;
	}
}

