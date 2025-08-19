/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 22:33:21 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/19 20:18:16 by mhasoneh         ###   ########.fr       */
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

t_token	*prepare_command(char *input, char ***envp, int arg_count)
{
	char	**args;
	t_token	*token;

	token = NULL;
	args = parse_arguments(input, &arg_count);
	if (!args)
		return (NULL);
	if (is_operator(args[0]))
		return (handle_operator(args, *envp));
	if (!command_exists(args[0], *envp))
	{
		set_shell_status(127);
		return (printf("%s: command not found\n", args[0]), NULL);
	}
	args = expand(args, *envp);
	if (!args)
		return (NULL);
	if (check_syntax_error(args))
		return (set_shell_status(2), NULL);
	tokenize(args, &token);
	ft_free_arr(args);
	return (token);
}

char	*read_prompt_line(void)
{
	char	cwd[1024];
	char	*prompt;
	char	*tmp1;
	char	*tmp2;
	char	*line;

	if (getcwd(cwd, sizeof(cwd)))
	{
		tmp1 = ft_strjoin("\033[1;94m", cwd);
		tmp2 = ft_strjoin(tmp1, "\033[0;37m");
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
		add_history(line);
		return (line);
	}
}

void	shell_loop(int arg_count, char ***envp)
{
	char			*input;
	t_token			*token;

	while (1)
	{
		input = get_input();
		if (!input)
			break;
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
