/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:16:14 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 17:39:24 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	expand_simple_var(t_expand_ctx *ctx)
{
	char	var[256];
	int		k;
	char	*val;
	int		len;

	k = 0;
	ctx->i++; // skip $
	while (ctx->arg[ctx->i] && (ft_isalnum(ctx->arg[ctx->i]) || ctx->arg[ctx->i] == '_') && k < 255)
		var[k++] = ctx->arg[ctx->i++];
	var[k] = '\0';
	val = lookup_env(var, ctx->envp);
	if (val)
	{
		len = ft_strlen(val);
		ft_strcpy(ctx->buffer + ctx->j, val);
		ctx->j += len;
	}
	return (ctx->j);
}

int	expand_braced_var(t_expand_ctx *ctx)
{
	char	var[256];
	int		k;
	char	*val;
	int		len;

	k = 0;
	ctx->i += 2; // skip ${
	while (ctx->arg[ctx->i] && ctx->arg[ctx->i] != '}' && k < 255)
		var[k++] = ctx->arg[ctx->i++];
	var[k] = '\0';
	if (ctx->arg[ctx->i] == '}')
		ctx->i++;
	val = lookup_env(var, ctx->envp);
	if (val)
	{
		len = ft_strlen(val);
		ft_strcpy(ctx->buffer + ctx->j, val);
		ctx->j += len;
	}
	return (ctx->j);
}

int	process_variable_expansion(t_expand_ctx *ctx)
{
	int	result;

	if (ctx->arg[ctx->i] == '$' && ctx->arg[ctx->i + 1])
	{
		result = expand_exit_status(ctx);
		if (result != 0)
			return (result);
		result = expand_pid(ctx);
		if (result != 0)
			return (result);
		if (ctx->arg[ctx->i + 1] == '{')
			return (expand_braced_var(ctx));
		if (ft_isalnum(ctx->arg[ctx->i + 1]) || ctx->arg[ctx->i + 1] == '_')
			return (expand_simple_var(ctx));
		ctx->buffer[ctx->j++] = ctx->arg[ctx->i++];
		return (ctx->j);
	}
	ctx->buffer[ctx->j++] = ctx->arg[ctx->i++];
	return (ctx->j);
}

char	*expand_variables_in_string(char *str, char **envp)
{
	char			buffer[BUFFER_SIZE * 4];
	t_expand_ctx	ctx;

	ctx.i = 0;
	ctx.j = 0;
	ctx.arg = str;
	ctx.buffer = buffer;
	ctx.envp = envp;
	
	while (ctx.arg[ctx.i])
		ctx.j = process_variable_expansion(&ctx);
	ctx.buffer[ctx.j] = '\0';
	return (ft_strdup(ctx.buffer));
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
