/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:30:47 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 20:53:32 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void shell_loop(int arg_count, char ***envp)
{
	char    *input;
	char    **args;
	t_token *token;

	while (1)
	{
		g_signal = 0;
		token = NULL;
		args = NULL;
		input = get_input();
		if (input == NULL)
			break ;
		if (*input == '\0')
		{
		// free(input);
			continue ;
		}
		args = parse_arguments(input, &arg_count);
		if (!args)
		{
			//free(input);
			continue;
		}
		args = expand(args, *envp);
		if (!check_syntax_error(args))
			tokenize(args, &token);
		handle_command(token, envp);
		
		// Fix: Always free args and tokens
	// if (args)
		//    ft_free_arr((void *)&args);
		//free_tokens(token);
		//free(input);
	}
}

char *get_input(void)
{
	char *line;

	while (1)
	{
		line = read_prompt_line();
		if (!line)
		{
			printf("\033[0;31mexit\n\033[0m");
			return (NULL);
		}
		if (*line == '\0')
		{
			//free(line);
			continue;
		}
		line = append_until_quotes_closed(line);
		if (!line)
			continue;
		add_history(line);
		return (line);
	}
}

char *read_prompt_line(void)
{
	char cwd[1024];
	char *prompt;
	char *tmp1;
	char *tmp2;
	char *line;

	if (getcwd(cwd, sizeof(cwd)))
	{
		tmp1 = ft_strjoin("\033[1;94m", cwd);
		tmp2 = ft_strjoin(tmp1, "\033[0;37m");
		//free(tmp1);
		prompt = ft_strjoin(tmp2, "$ ");
		//free(tmp2);
	}
	else
		prompt = ft_strdup("$ ");
	line = readline(prompt);
	//free(prompt);
	return (line);
}
