/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:30:47 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 05:22:00 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	process_input(t_shell *shell, char *input)
{
	/* Tokenize input */
	shell->tokens = tokenize(input);
	if (!shell->tokens)
		return;
	
	/* Expand variables and quotes */
	expand_tokens(shell, shell->tokens);
	
	/* Validate syntax */
	if (!validate_syntax(shell->tokens))
	{
		shell->exit_status = EXIT_MISUSE;
		return;
	}
	
	/* Parse commands */
	shell->commands = parse_commands(shell->tokens);
	if (!shell->commands)
	{
		shell->exit_status = EXIT_FAILURE;
		return;
	}
	
	/* Execute commands */
	execute_commands(shell, shell->commands);
}

void	shell_loop(t_shell *shell)
{
	char	*input;
	char	*prompt;

	while (!shell->exit_flag)
	{
		/* Reset signal flag */
		g_signal = 0;
		
		/* Get and display prompt */
		prompt = get_prompt(shell);
		input = readline(prompt);
		free(prompt);
		
		/* Handle EOF (Ctrl+D) */
		if (!input)
		{
			printf("exit\n");
			shell->exit_flag = 1;
			break;
		}
		
		/* Handle signal interruption */
		if (g_signal)
		{
			shell->exit_status = EXIT_CTRL_C;
			free(input);
			continue;
		}
		
		/* Skip empty input */
		if (!*input)
		{
			free(input);
			continue;
		}
		
		/* Add to history */
		add_history(input);
		
		/* Process input */
		process_input(shell, input);
		
		/* Cleanup for next iteration */
		free(input);
		tokens_free(shell->tokens);
		commands_free(shell->commands);
		shell->tokens = NULL;
		shell->commands = NULL;
	}
}
