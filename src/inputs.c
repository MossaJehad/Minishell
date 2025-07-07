/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:53:51 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/07/07 12:55:02 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_builtin(char *input, char **args, int arg_count,
						t_token *token, t_data *data)
{
	char	cwd[1024];
	char	*cmd;

	cmd = args[0];
	if (ft_strcmp(cmd, "exit") == 0)
		handle_exit_command(args, data);
	else if (ft_strcmp(cmd, "echo") == 0)
		handle_echo_command(token);
	else if (ft_strcmp(cmd, "cd") == 0)
		handle_cd_command(input + 2, arg_count, data);
	else if (ft_strcmp(cmd, "env") == 0)
		handle_env_command(args, data);
	else if (ft_strcmp(cmd, "unset") == 0)
		handle_unset_command(args, data);
	else if (ft_strcmp(cmd, "export") == 0)
		handle_export_command(args, data);
	else if (ft_strcmp(cmd, "type") == 0 && arg_count > 1)
		handle_type_command(args[1], data);
	else if (ft_strcmp(cmd, "pwd") == 0)
		printf("%s\n", getcwd(cwd, sizeof(cwd)));
}

void	execute_command(char *cmd, char **args, t_data *data)
{
	char	*full_path;
	pid_t	pid;
	int		status;

	full_path = found_commands(cmd);
	pid = fork();
	if (pid == 0)
	{
		set_signals_noninteractive();
		execve(full_path, args, data->env);
		perror("execve");
		exit(1);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		data->last_status = WEXITSTATUS(status);
	}
	else
	{
		perror("fork");
		data->last_status = 1;
	}
	free(full_path);
}

void	handle_command(char *input, char **args, int arg_count,
						t_token *token, t_data *data)
{
	char	*cmd;
	char	*found;

	cmd = args[0];
	if (ft_strcmp(cmd, "exit") == 0 || ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0 || ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "type") == 0)
	{
		execute_builtin(input, args, arg_count, token, data);
		return ;
	}
	found = found_commands(cmd);
	if (found)
	{
		free(found);
		execute_command(cmd, args, data);
	}
	else
	{
		printf("%s: command not found\n", input);
		data->last_status = 127;
	}
}

char	*get_input(t_data *data)
{
	char	*line;

	line = readline("$ ");
	if (line == NULL)
	{
		printf("exit\n");
		exit(data->last_status);
	}
	if (line && *line)
		add_history(line);
	return (line);
}

char	*search_commands(const char *input)
{
	char	*path_env;
	char	*dir;
	char	*full_path;

	if (is_shell_builtin(input))
		return (NULL);
	path_env = ft_strdup(getenv("PATH"));
	if (path_env)
	{
		dir = ft_strtok(path_env, ":");
		while (dir != NULL)
		{
			full_path = path_join(dir, input);
			if (access(full_path, X_OK) == 0)
			{
				free(path_env);
				return (full_path);
			}
			free(full_path);
			dir = ft_strtok(NULL, ":");
		}
		free(path_env);
	}
	return (NULL);
}

char	*found_commands(const char *input)
{
	char	*arr;

	arr = search_commands(input);
	return (arr);
}
