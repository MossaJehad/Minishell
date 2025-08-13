/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:18:28 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 05:10:06 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//int	handle_dollar_expansion(t_expand_ctx *ctx)
//{
//	int	result;

//	if (ctx->arg[ctx->i] == '$' && ctx->arg[ctx->i + 1])
//	{
//		result = expand_special_vars(ctx);
//		if (result != 0)
//			return (result);
//		if (ctx->arg[ctx->i + 1] == '{')
//			return (expand_braced_var(ctx));
//		if (ft_isalnum(ctx->arg[ctx->i + 1]) || ctx->arg[ctx->i + 1] == '_')
//			return (expand_simple_var(ctx));
//		return (copy_char(ctx->arg, &ctx->i, ctx->buffer, ctx->j));
//	}
//	return (0);
//}

//char	**expand(char **args, char **envp)
//{
//	int		i;
//	char	*new;

//	i = 0;
//	while (args[i])
//	{
//		if (args[i][0] == '\'' && args[i][ft_strlen(args[i]) - 1] == '\'')
//		{
//			args[i] = expand_single_quote(args[i]);
//		}
//		else if (args[i][0] == '"' && args[i][ft_strlen(args[i]) - 1] == '"')
//		{
//			args[i] = expand_double_quote(args[i], envp);
//		}
//		else if (ft_strchr(args[i], '$'))
//		{
//			new = expand_variables_in_string(args[i], envp);
//			free(args[i]);
//			args[i] = new;
//		}
//		i++;
//	}
//	return (args);
//}

int	expand_pid(t_expand_ctx *ctx)
{
	char	*pid_str;
	pid_t	pid;
	int		len;

	if (ctx->arg[ctx->i + 1] == '$')
	{
		pid = getpid();
		pid_str = malloc(20);
		if (!pid_str)
			return (0);
		sprintf(pid_str, "%d", pid);
		len = ft_strlen(pid_str);
		if (ctx->j + len < BUFFER_SIZE * 4 - 1)
		{
			ft_strcpy(ctx->buffer + ctx->j, pid_str);
			ctx->j += len;
		}
		free(pid_str);
		ctx->i += 2;
		return (ctx->j);
	}
	return (0);
}

char	*expand_variable(t_shell *shell, const char *var_name)
{
	char	*value;
	char	*exit_status_str;

	if (!var_name)
		return (safe_strdup(""));

	/* Handle special variables */
	if (ft_strcmp(var_name, "?") == 0)
	{
		exit_status_str = ft_itoa(shell->exit_status);
		return (exit_status_str ? exit_status_str : safe_strdup("0"));
	}
	else if (ft_strcmp(var_name, "$") == 0)
	{
		exit_status_str = ft_itoa(getpid());
		return (exit_status_str ? exit_status_str : safe_strdup("0"));
	}

	/* Regular environment variable */
	value = env_get(shell, var_name);
	return (value ? safe_strdup(value) : safe_strdup(""));
}

char	*extract_var_name(const char *str, int *pos)
{
	int	start;
	int	end;

	start = *pos;
	end = start;

	/* Handle special single-character variables */
	if (str[end] == '?' || str[end] == '$')
	{
		end++;
		*pos = end;
		return (ft_strndup(str + start, 1));
	}

	/* Handle braced variables ${VAR} */
	if (str[end] == '{')
	{
		start++;
		end++;
		while (str[end] && str[end] != '}')
			end++;
		if (str[end] == '}')
		{
			*pos = end + 1;
			return (ft_strndup(str + start, end - start));
		}
		/* Invalid syntax, treat as literal */
		*pos = start;
		return (NULL);
	}

	/* Regular variable name */
	while (str[end] && (ft_isalnum(str[end]) || str[end] == '_'))
		end++;

	*pos = end;
	if (end == start)
		return (NULL);
	
	return (ft_strndup(str + start, end - start));
}

