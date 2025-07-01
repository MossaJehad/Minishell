/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhaddadi <jhaddadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:26:55 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/07/01 17:53:12 by jhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_shell_builtin(const char *cmd)
{
	if (ft_strcmp(cmd, "type") == 0 || ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "exit") == 0 || ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "unset") == 0)
	{
		return (1);
	}
	return (0);
}

void	system_handler(char *input)
{
	size_t	len;

	len = strlen(input);
	if (input[len - 1] == '\n')
		input[len - 1] = '\0';
	system(input);
}

void	print_welcome_banner(void)
{
	// printf("┏┓┳┏┓┳┳┓┏┓\n");
	// printf("┗┓┃┃┓┃┃┃┣┫\n");
	// printf("┗┛┻┗┛┛ ┗┛┗ .\n");
	// printf("			\n");
}

void	init_shell(void)
{
	setbuf(stdout, NULL);
	print_welcome_banner();
}

//void	prompt(void)
//{
//	// char cwd[1024];
//	// if (getcwd(cwd, sizeof(cwd)) != NULL)
//	// printf("%s$ ", cwd);
//	// else
//	printf("$ ");
//}
