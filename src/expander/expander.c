/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:18:28 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/28 22:09:37 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_dollar_expansion(t_expand_ctx *ctx)
{
	int	result;

	if (ctx->input[ctx->in_idx] == '$' && ctx->input[ctx->in_idx + 1])
	{
		result = expand_special_vars(ctx);
		if (result != 0)
			return (result);
		if (ctx->input[ctx->in_idx + 1] == '{')
			return (expand_braced_var(ctx));
		if (ft_isalnum(ctx->input[ctx->in_idx + 1])
			|| ctx->input[ctx->in_idx + 1] == '_')
			return (expand_simple_var(ctx));
		return (copy_char(ctx->input, &ctx->in_idx, ctx->output, ctx->out_idx));
	}
	return (0);
}

static char	*handle_argument(char *arg, char **envp, int index)
{
	char	*new;
	char	*trimmed;
	int		was_quoted;
	size_t	len;

	was_quoted = 0;
	len = ft_strlen(arg);
	if (arg[0] == '\'' && arg[len - 1] == '\'')
		return (expand_single_quote(arg));
	else if (arg[0] == '"' && arg[len - 1] == '"')
		return (expand_double_quote(arg, envp));
	else if (ft_strchr(arg, '$'))
	{
		new = expand_variables_in_string(arg, envp);
		if (index == 0 && new)
		{
			trimmed = trim_whitespace(new);
			free(new);
			new = trimmed;
		}
		free(arg);
		return (new);
	}
	return (arg);
}

char	**expand(char **args, char **envp)
{
	int		i;
	char	*new_arg;

	i = -1;
	while (args[++i])
	{
		new_arg = handle_argument(args[i], envp, i);
		if (new_arg != args[i])
			args[i] = new_arg;
	}
	args = apply_word_splitting(args, envp);
	return (args);
}

int	expand_pid(t_expand_ctx *ctx)
{
	char	pid_str[20];
	pid_t	pid;
	int		len;

	if (ctx->input[ctx->in_idx + 1] == '$')
	{
		pid = fork();
		if (pid == 0)
			exit(0);
		sprintf(pid_str, "%d", pid);
		len = ft_strlen(pid_str);
		if (ctx->out_idx + len < BUFFER_SIZE * 4 - 1)
		{
			ft_strcpy(ctx->output + ctx->out_idx, pid_str);
			ctx->out_idx += len;
		}
		ctx->in_idx += 2;
		return (ctx->out_idx);
	}
	return (0);
}
