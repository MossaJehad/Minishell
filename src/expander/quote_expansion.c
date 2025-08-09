/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:57:26 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 16:22:40 by mhasoneh         ###   ########.fr       */
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
	t_expand_ctx	ctx;

	ctx.i = 1;
	ctx.j = 0;
	ctx.arg = arg;
	ctx.buffer = buffer;
	ctx.envp = envp;
	
	while (ctx.arg[ctx.i] && ctx.arg[ctx.i] != '"')
	{
		int result = handle_dollar_expansion(&ctx);
		if (result != 0)
			continue;
		ctx.j = copy_char(ctx.arg, &ctx.i, ctx.buffer, ctx.j);
	}
	ctx.buffer[ctx.j] = '\0';
	return ft_strdup(ctx.buffer);
}

char	*expand_double_quote(char *arg, char **envp)
{
	char	*result;

	result = expand_double_quote_loop(arg, envp);
	free(arg);
	return (result);
}

char	*expand_single_quote(char *arg)
{
	char	*str;
	int		i;
	int		j;

	i = 1;
	j = 0;
	if (ft_strlen(arg) < 2)
		return (ft_strdup(""));
	str = malloc(ft_strlen(arg) - 1);
	if (!str)
	{
		ft_free((void **)&str);
		return (NULL);
	}
	while (i < (int)ft_strlen(arg) - 1)
	{
		str[j++] = arg[i++];
	}
	str[j] = '\0';
	free(arg);
	return (str);
}
