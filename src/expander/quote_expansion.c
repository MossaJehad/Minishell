/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:57:26 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 05:36:56 by mhasoneh         ###   ########.fr       */
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

//char	*expand_double_quote_loop(char *arg, char **envp)
//{
//	char			buffer[BUFFER_SIZE];
//	t_expand_ctx	ctx;
//	int				result;

//	ctx.i = 1;
//	ctx.j = 0;
//	ctx.arg = arg;
//	ctx.buffer = buffer;
//	ctx.envp = envp;
//	while (ctx.arg[ctx.i] && ctx.arg[ctx.i] != '"' && ctx.j < BUFFER_SIZE - 1)
//	{
//		result = handle_dollar_expansion(&ctx);
//		if (result != 0)
//			continue ;
//		ctx.j = copy_char(ctx.arg, &ctx.i, ctx.buffer, ctx.j);
//	}
//	ctx.buffer[ctx.j] = '\0';
//	return (ft_strdup(ctx.buffer));
//}

//char	*expand_double_quote(char *arg, char **envp)
//{
//	char	*result;

//	result = expand_double_quote_loop(arg, envp);
//	free(arg);
//	return (result);
//}

//char	*expand_single_quote(char *arg)
//{
//	char	*str;
//	int		len;
//	int		i;
//	int		j;

//	if (!arg || ft_strlen(arg) < 2)
//	{
//		free(arg);
//		return (ft_strdup(""));
//	}
//	len = ft_strlen(arg);
//	str = malloc(len - 1);
//	if (!str)
//	{
//		free(arg);
//		return (NULL);
//	}
//	i = 1;
//	j = 0;
//	while (i < len - 1)
//		str[j++] = arg[i++];
//	str[j] = '\0';
//	free(arg);
//	return (str);
//}

char	*expand_variables(t_shell *shell, const char *str)
{
	char	*result;
	char	*var_name;
	char	*var_value;
	char	*temp;
	int		i;
	int		start;

	if (!str)
		return (NULL);

	result = safe_strdup("");
	i = 0;
	start = 0;

	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			/* Add text before variable */
			if (i > start)
			{
				temp = ft_strndup(str + start, i - start);
				result = ft_strjoin_free(result, temp);
				free(temp);
			}

			/* Extract and expand variable */
			i++; /* Skip $ */
			var_name = extract_var_name(str, &i);
			if (var_name)
			{
				var_value = expand_variable(shell, var_name);
				result = ft_strjoin_free(result, var_value);
				free(var_name);
				free(var_value);
			}
			else
			{
				/* No valid variable name, keep the $ */
				result = ft_strjoin_free(result, "$");
			}
			start = i;
		}
		else
		{
			i++;
		}
	}

	/* Add remaining text */
	if (i > start)
	{
		temp = ft_strndup(str + start, i - start);
		result = ft_strjoin_free(result, temp);
		free(temp);
	}

	return (result);
}

char	*expand_quotes(const char *str)
{
	char	*result;
	char	quote_char;
	int		in_quotes;
	int		i;
	int		j;
	int		len;

	if (!str)
		return (NULL);

	len = ft_strlen(str);
	result = (char *)safe_malloc(len + 1);
	
	i = 0;
	j = 0;
	in_quotes = 0;
	quote_char = 0;

	while (str[i])
	{
		if (!in_quotes && (str[i] == '\'' || str[i] == '"'))
		{
			quote_char = str[i];
			in_quotes = 1;
		}
		else if (in_quotes && str[i] == quote_char)
		{
			in_quotes = 0;
			quote_char = 0;
		}
		else
		{
			result[j++] = str[i];
		}
		i++;
	}

	result[j] = '\0';
	return (result);
}

char	*expand_token_value(t_shell *shell, const char *value)
{
	char	*expanded;
	char	*unquoted;

	if (!value)
		return (NULL);

	/* First expand variables (but not inside single quotes) */
	expanded = expand_variables_with_quotes(shell, value);
	
	/* Then remove quotes */
	unquoted = expand_quotes(expanded);
	
	free(expanded);
	return (unquoted);
}

char	*expand_variables_with_quotes(t_shell *shell, const char *str)
{
	char	*result;
	char	*temp;
	int		i;
	int		start;
	int		in_single_quotes;

	if (!str)
		return (NULL);

	result = safe_strdup("");
	i = 0;
	start = 0;
	in_single_quotes = 0;

	while (str[i])
	{
		if (str[i] == '\'' && !in_single_quotes)
		{
			/* Add text before quote and enter single quote mode */
			if (i > start)
			{
				temp = expand_variables(shell, ft_strndup(str + start, i - start));
				result = ft_strjoin_free(result, temp);
				free(temp);
			}
			in_single_quotes = 1;
			result = ft_strjoin_free(result, "'");
			i++;
			start = i;
		}
		else if (str[i] == '\'' && in_single_quotes)
		{
			/* Add literal text and exit single quote mode */
			if (i > start)
			{
				temp = ft_strndup(str + start, i - start);
				result = ft_strjoin_free(result, temp);
				free(temp);
			}
			in_single_quotes = 0;
			result = ft_strjoin_free(result, "'");
			i++;
			start = i;
		}
		else
		{
			i++;
		}
	}

	/* Handle remaining text */
	if (i > start)
	{
		if (in_single_quotes)
		{
			/* Literal text in single quotes */
			temp = ft_strndup(str + start, i - start);
			result = ft_strjoin_free(result, temp);
			free(temp);
		}
		else
		{
			/* Expand variables in remaining text */
			char	*t = ft_strndup(str + start, i - start);
			temp = expand_variables(shell, t);
			result = ft_strjoin_free(result, temp);
			free(temp);
			free(t);
		}
	}

	return (result);
}

void	expand_tokens(t_shell *shell, t_token *tokens)
{
	t_token	*current;
	char	*expanded;

	current = tokens;
	while (current)
	{
		if (current->value && (current->type == TOKEN_WORD || 
			current->type == TOKEN_COMMAND))
		{
			expanded = expand_token_value(shell, current->value);
			if (expanded)
			{
				free(current->value);
				current->value = expanded;
			}
		}
		current = current->next;
	}
}
