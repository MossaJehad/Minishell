/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 16:01:58 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/30 16:02:06 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	run_echo_builtin(char *cmd_argv[MAX_ARGS], int cmd_argc)
{
	t_token	*cmd_token;
	int		k;

	cmd_token = NULL;
	create_token(&cmd_token, cmd_argv[0], "command");
	k = 1;
	while (k < cmd_argc)
		create_token(&cmd_token, cmd_argv[k++], "word");
	handle_echo_command(cmd_token);
	free_tokens(cmd_token);
}

void	run_pwd_builtin(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
		perror("pwd");
}

int	process_token_and_redirects(t_token **cur,
	char *cmd_argv[MAX_ARGS], int *cmd_argc)
{
	while (*cur && (*cur)->type != PIPE)
	{
		if ((*cur)->type == REDIRECT || (*cur)->type == REDIRECT_OUT
			|| (*cur)->type == APPEND)
		{
			if (handle_redirect_tokens(cur) == -1)
				return (-1);
			continue ;
		}
		if ((*cur)->type == HEREDOC)
		{
			skip_heredoc_tokens(cur);
			continue ;
		}
		if ((*cur)->type == WORD || (*cur)->type == COMMAND
			|| (*cur)->type == QUOTED_STRING)
			cmd_argv[(*cmd_argc)++] = (*cur)->value;
		*cur = (*cur)->next;
	}
	return (0);
}
