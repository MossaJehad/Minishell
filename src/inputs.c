/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:19:51 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/05/26 17:20:31 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	handle_command(char *input, char **args, int arg_count,
						t_token *token, t_data *data)
{
	char	cwd[1024];
	char	*cmd;

	cmd = args[0];
	if (ft_strcmp(cmd, "exit") == 0 && arg_count > 1
		&& ft_strcmp(args[1], "0") == 0)
		exit(0);
	else if (ft_strcmp(cmd, "echo") == 0)
		handle_echo_command(token);
	else if (ft_strcmp(cmd, "clear") == 0)
		system("clear");
	else if (ft_strcmp(cmd, "ls") == 0)
		handle_ls_command(args, data);
	else if (ft_strcmp(cmd, "cat") == 0)
		handle_cat_command(args, data);
	else if (ft_strcmp(cmd, "type") == 0 && arg_count > 1)
		handle_type_command(args[1], data);
	else if (ft_strcmp(cmd, "pwd") == 0)
		printf("%s\n", getcwd(cwd, sizeof(cwd)));
	else if (ft_strcmp(cmd, "cd") == 0)
		handle_cd_command(input + 2, arg_count, data);
	else if (arg_count == 2)
		system_handler(input);
	else
		printf("%s: command not found\n", input);
}

char	*get_input(void)
{
	char	*line;

	line = readline("$ ");
	if (line == NULL)
	{
		printf("\n");
		return (NULL);
	}
	if (line && *line)
		add_history(line);
	return (line);
}
