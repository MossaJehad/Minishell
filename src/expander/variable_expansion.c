/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:16:14 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/22 10:50:09 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	expand_simple_var(t_expand_ctx *ctx)
{
	char	var[256];
	char	*val;
	int		k;
	int		len;

	k = 0;
	ctx->in_idx++;
	if (ft_isdigit(ctx->input[ctx->in_idx]))
		var[k++] = ctx->input[ctx->in_idx++];
	else
	{
		while (ctx->input[ctx->in_idx]
			&& (ft_isalnum(ctx->input[ctx->in_idx])
			|| ctx->input[ctx->in_idx] == '_') && k < 255)
				var[k++] = ctx->input[ctx->in_idx++];
	}
	var[k] = '\0';
	val = lookup_env(var, ctx->envp);
	if (val)
	{
		len = ft_strlen(val);
		ft_strcpy(ctx->output + ctx->out_idx, val);
		ctx->out_idx += len;
	}
	return (ctx->out_idx);
}

int	expand_braced_var(t_expand_ctx *ctx)
{
	char	var[256];
	char	*val;
	int		k;
	int		len;

	k = 0;
	ctx->in_idx += 2;
	while (ctx->input[ctx->in_idx] && ctx->input[ctx->in_idx] != '}' && k < 255)
		var[k++] = ctx->input[ctx->in_idx++];
	var[k] = '\0';
	if (ctx->input[ctx->in_idx] == '}')
		ctx->in_idx++;
	val = lookup_env(var, ctx->envp);
	if (val)
	{
		len = ft_strlen(val);
		ft_strcpy(ctx->output + ctx->out_idx, val);
		ctx->out_idx += len;
	}
	return (ctx->out_idx);
}

int expand_exit_status(t_expand_ctx *ctx)
{
	char	exit_status_str[20];
	int		len;

	if (ctx->input[ctx->in_idx + 1] == '?')
	{
		ft_itoa_buf(get_shell_status(), exit_status_str, 0, 0);
		len = ft_strlen(exit_status_str);
		if (ctx->out_idx + len < BUFFER_SIZE * 4 - 1)
		{
			ft_strcpy(ctx->output + ctx->out_idx, exit_status_str);
			ctx->out_idx += len;
		}
		ctx->in_idx += 2;
		return (ctx->out_idx);
	}
	if (ctx->input[ctx->in_idx + 1] == '0')
	{
		len = ft_strlen("minishell");
		if (ctx->out_idx + len < BUFFER_SIZE * 4 - 1)
		{
			ft_strcpy(ctx->output + ctx->out_idx, "minishell");
			ctx->out_idx += len;
		}
		ctx->in_idx += 2;
		return (ctx->out_idx);
	}
	return (0);
}

int	process_variable_expansion(t_expand_ctx *ctx)
{
	int	result;

	if (ctx->input[ctx->in_idx] == '$' && ctx->input[ctx->in_idx + 1])
	{
		result = expand_exit_status(ctx);
		if (result != 0)
			return (result);
		result = expand_pid(ctx);
		if (result != 0)
			return (result);
		if (ctx->input[ctx->in_idx + 1] == '{')
			return (expand_braced_var(ctx));
		if (ft_isalnum(ctx->input[ctx->in_idx + 1]) || ctx->input[ctx->in_idx + 1] == '_')
			return (expand_simple_var(ctx));
		return (copy_char(ctx->input, &ctx->in_idx, ctx->output, ctx->out_idx));
	}
	return (copy_char(ctx->input, &ctx->in_idx, ctx->output, ctx->out_idx));
}

int	expand_special_vars(t_expand_ctx *ctx)
{
	int	result;

	result = expand_exit_status(ctx);
	if (result != 0)
		return (result);
	result = expand_pid(ctx);
	if (result != 0)
		return (result);
	return (0);
}
