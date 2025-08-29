/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 22:33:21 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/29 16:25:36 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*read_prompt_line(void)
{
	char	cwd[MAX_ARGS];
	char	*prompt;
	char	*tmp1;
	char	*tmp2;
	char	*line;

	if (getcwd(cwd, sizeof(cwd)))
	{
		tmp1 = ft_strjoin("\001\033[1;94m\002", cwd);
		tmp2 = ft_strjoin(tmp1, "\001\033[0;37m\002");
		free(tmp1);
		prompt = ft_strjoin(tmp2, "$ ");
		free(tmp2);
	}
	else
		prompt = ft_strdup("$ ");
	line = readline(prompt);
	free(prompt);
	return (line);
}

char	*get_input(void)
{
	char	*line;

	ignore_signals();
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
			free(line);
			continue ;
		}
		line = append_until_quotes_closed(line);
		if (!line)
			continue ;
		if (line && !is_blank(line))
			add_history(line);
		return (line);
	}
}

void	shell_loop(int arg_count, char ***envp)
{
	char	*input;
	t_token	*token;

	while (1)
	{
		input = get_input();
		if (!input)
			break ;
		token = prepare_command(input, envp, arg_count);
		free(input);
		input = NULL;
		if (token)
		{
			handle_command(token, envp);
			free_tokens(token);
			token = NULL;
		}
	}
}
