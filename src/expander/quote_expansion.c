/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:57:26 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/30 15:23:15 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*lookup_env(const char *name, char **envp)
{
	size_t	len;
	int		i;

	len = ft_strlen(name);
	i = 0;
	while (envp && envp[i])
	{
		if (!ft_strncmp(envp[i], name, len) && envp[i][len] == '=')
			return (&envp[i][len + 1]);
		i++;
	}
	return (NULL);
}

char	*expand_double_quote_loop(char *arg, char **envp)
{
	char			buffer[BUFFER_SIZE];
	int				result;
	t_expand_ctx	ctx;

	ctx.input = arg;
	ctx.output = buffer;
	ctx.in_idx = 1;
	ctx.out_idx = 0;
	ctx.envp = envp;
	while (ctx.input[ctx.in_idx] && ctx.input[ctx.in_idx] != '"'
		&& ctx.out_idx < BUFFER_SIZE - 1)
	{
		result = handle_dollar_expansion(&ctx);
		if (result != 0)
			continue ;
		ctx.out_idx = copy_char(ctx.input, &ctx.in_idx, ctx.output,
				ctx.out_idx);
	}
	ctx.output[ctx.out_idx] = '\0';
	return (ft_strdup(ctx.output));
}

char	*expand_double_quote(char *arg, char **envp)
{
	char	*result;

	if (ft_strcmp(arg, "\"\"") == 0)
	{
		free(arg);
		return (ft_strdup(""));
	}
	result = expand_double_quote_loop(arg, envp);
	free(arg);
	return (result);
}

char	*extract_inner_single_quote(const char *arg, int len)
{
	char	*str;
	int		i;
	int		j;

	str = ft_calloc(len - 1, sizeof(char));
	if (!str)
		return (NULL);
	i = 1;
	j = 0;
	while (i < len - 1)
		str[j++] = arg[i++];
	str[j] = '\0';
	return (str);
}

char	*expand_single_quote(char *arg)
{
	char	*str;
	int		len;

	if (!arg)
		return (ft_strdup(""));
	len = ft_strlen(arg);
	if (ft_strcmp(arg, "''") == 0 || len < 2)
	{
		free(arg);
		return (ft_strdup(""));
	}
	str = extract_inner_single_quote(arg, len);
	free(arg);
	return (str);
}

char	*expand_variables_in_string(char *str, char **envp)
{
	char			buffer[BUFFER_SIZE * 4];
	t_expand_ctx	ctx;

	ctx.input = str;
	ctx.output = buffer;
	ctx.in_idx = 0;
	ctx.out_idx = 0;
	ctx.envp = envp;
	while (ctx.input[ctx.in_idx])
		ctx.out_idx = process_variable_expansion(&ctx);
	ctx.output[ctx.out_idx] = '\0';
	return (ft_strdup(ctx.output));
}
