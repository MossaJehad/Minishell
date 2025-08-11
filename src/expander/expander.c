/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:18:28 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/11 11:00:36 by zal-qais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_dollar_expansion(t_expand_ctx *ctx)
{
	int	result;

	if (ctx->arg[ctx->i] == '$' && ctx->arg[ctx->i + 1])
	{
		result = expand_special_vars(ctx);
		if (result != 0)
			return (result);
		if (ctx->arg[ctx->i + 1] == '{')
			return (expand_braced_var(ctx));
		if (ft_isalnum(ctx->arg[ctx->i + 1]) || ctx->arg[ctx->i + 1] == '_')
			return (expand_simple_var(ctx));
		return (copy_char(ctx->arg, &ctx->i, ctx->buffer, ctx->j));
	}
	return (0);
}

char	**expand(char **args, char **envp)
{
	int		i;
	char	*new;

	i = 0;
	while (args[i])
	{
		if (args[i][0] == '\'' && args[i][ft_strlen(args[i]) - 1] == '\'')
		{
			args[i] = expand_single_quote(args[i]);
		}
		else if (args[i][0] == '"' && args[i][ft_strlen(args[i]) - 1] == '"')
		{
			args[i] = expand_double_quote(args[i], envp);
		}
		else if (ft_strchr(args[i], '$'))
		{
			new = expand_variables_in_string(args[i], envp);
			free(args[i]);
			args[i] = new;
		}
		i++;
	}
	return (args);
}
// int	handle_dollar_expansion(t_expand_ctx *ctx)
// {
// 	int	result;

// 	if (ctx->arg[ctx->i] == '$' && ctx->arg[ctx->i + 1])
// 	{
// 		result = expand_special_vars(ctx);
// 		if (result != 0)
// 			return (result);
// 		if (ctx->arg[ctx->i + 1] == '{')
// 			return (expand_braced_var(ctx));
// 		if (ft_isalnum(ctx->arg[ctx->i + 1]) || ctx->arg[ctx->i + 1] == '_')
// 			return (expand_simple_var(ctx));
// 		return (copy_char(ctx->arg, &ctx->i, ctx->buffer, ctx->j));
// 	}
// 	return (0);
// }

// char	**expand(char **args, char **envp)
// {
// 	int		i;
// 	char	*new;

// 	i = 0;
// 	while (args[i])
// 	{
// 		if (args[i][0] == '\'' && args[i][ft_strlen(args[i]) - 1] == '\'')
// 		{
// 			args[i] = expand_single_quote(args[i]);
// 		}
// 		else if (args[i][0] == '"' && args[i][ft_strlen(args[i]) - 1] == '"')
// 		{
// 			args[i] = expand_double_quote(args[i], envp);
// 		}
// 		else if (ft_strchr(args[i], '$'))
// 		{
// 			new = expand_variables_in_string(args[i], envp);
// 			free(args[i]);
// 			args[i] = new;
// 		}
// 		i++;
// 	}
// 	return (args);
// }
