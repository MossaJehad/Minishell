/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:26:44 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 05:19:20 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);

	return (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0 ||
		ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0 ||
		ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0 ||
		ft_strcmp(cmd, "exit") == 0);
}

int	execute_builtin(t_shell *shell, t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (EXIT_FAILURE);

	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (builtin_echo(shell, cmd->args));
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (builtin_cd(shell, cmd->args));
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (builtin_pwd(shell, cmd->args));
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (builtin_export(shell, cmd->args));
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (builtin_unset(shell, cmd->args));
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (builtin_env(shell, cmd->args));
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (builtin_exit(shell, cmd->args));

	return (EXIT_FAILURE);
}


int	should_run_in_parent(const char *cmd)
{
	if (!cmd)
		return (0);
	
	return (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "export") == 0 ||
		ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "exit") == 0);
}

//void	cleanup_shell_resources(char **env, t_token *token, char **args,
//		char *input)
//{
//	if (env)
//		ft_free_arr((void ***)&env);
//	if (token)
//		free_tokens(token);
//	if (args)
//		ft_free_arr((void *)&args);
//	if (input)
//		free(input);
//	rl_clear_history();
//}

int	print_exported_vars(t_shell *shell)
{
	char	**env_array;
	char	**sorted_env;
	char	*equals;
	int		count;
	int		i;

	env_array = env_to_array(shell);
	count = 0;
	while (env_array[count])
		count++;

	/* Sort environment variables */
	sorted_env = (char **)safe_malloc(sizeof(char *) * (count + 1));
	i = 0;
	while (i < count)
	{
		sorted_env[i] = safe_strdup(env_array[i]);
		i++;
	}
	sorted_env[count] = NULL;

	sort_string_array(sorted_env);

	/* Print in export format */
	i = 0;
	while (sorted_env[i])
	{
		equals = ft_strchr(sorted_env[i], '=');
		if (equals)
		{
			*equals = '\0';
			printf("declare -x %s=\"%s\"\n", sorted_env[i], equals + 1);
		}
		else
		{
			printf("declare -x %s\n", sorted_env[i]);
		}
		i++;
	}

	free_string_array(env_array);
	free_string_array(sorted_env);
	return (EXIT_SUCCESS);
}

int	is_numeric(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);

	i = 0;
	if (str[i] == '+' || str[i] == '-')
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


void	close_pipe_fds(int *pipe_fds, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		close(pipe_fds[i * 2]);     /* Read end */
		close(pipe_fds[i * 2 + 1]); /* Write end */
		i++;
	}
}


//int	ft_strcmp_safe(const char *s1, const char *s2)
//{
//	if (!s1 && !s2)
//		return (0);
//	if (!s1)
//		return (-1);
//	if (!s2)
//		return (1);
	
//	return (ft_strcmp(s1, s2));
//}

//char	*trim_quotes(const char *str)
//{
//	int		len;
//	char	*result;

//	if (!str)
//		return (NULL);

//	len = ft_strlen(str);
	
//	/* Check if string is quoted */
//	if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"') ||
//		(str[0] == '\'' && str[len - 1] == '\'')))
//	{
//		result = ft_strndup(str + 1, len - 2);
//	}
//	else
//	{
//		result = safe_strdup(str);
//	}

//	return (result);
//}

void	sort_string_array(char **arr)
{
	int		i;
	int		j;
	char	*temp;

	if (!arr)
		return;

	i = 0;
	while (arr[i])
	{
		j = i + 1;
		while (arr[j])
		{
			if (ft_strcmp(arr[i], arr[j]) > 0)
			{
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
			j++;
		}
		i++;
	}
}