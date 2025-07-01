/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhaddadi <jhaddadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:43:37 by jhaddadi          #+#    #+#             */
/*   Updated: 2025/07/01 16:58:24 by jhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_dollar(char *arg, int last_status)
{
	char	*var_name;
	char	*value;
	char	*str;

	var_name = arg + 1;
	value = NULL;
	if (ft_strcmp(arg, "$?") == 0)
	{
		str = ft_itoa(last_status);
		return (str);
	}
	if (ft_strcmp(arg, "$0") == 0)
		return (ft_strdup("minishell"));
	//else if (ft_strcmp(arg, "$$") == 0)
	//i dont know
	else if (ft_strcmp(arg, "$") == 0)
		return (ft_strdup("$"));
	else
	{
		value = getenv(var_name);
		if (value)
			return (ft_strdup(value));
		return (ft_strdup(""));
	}
}

char	*expand_double_quote(char *arg, t_data *data)
{
	char	*str;
	char	*exp;

	str = ft_substr(arg, 1, ft_strlen(arg) - 2);
	free(arg);
	exp = expand_token(str, data);
	free(str);
	return (exp);
}

char	*expand_single_quote(char *arg)
{
	char	*str;
	int		i;
	int		j;
	int		len;

	len = ft_strlen(arg);
	i = 1;
	j = 0;
	if (len < 2)
		return (ft_strdup(""));
	str = malloc(len - 1);
	if (!str)
		return (NULL);
	while (i < len - 1)
		str[j++] = arg[i++];
	str[j] = '\0';
	free(arg);
	return (str);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (joined);
}

static int	get_var_end(const char *arg, int start)
{
	if (arg[start] == '?')
		return (start + 1);
	if (ft_isdigit(arg[start]))
		return (start + 1);
	if (ft_isalnum(arg[start]) || arg[start] == '_')
	{
		while (ft_isalnum(arg[start]) || arg[start] == '_')
			start++;
		return (start);
	}
	return (start);
}

static char	*get_expanded_var(const char *arg, int var_start, int var_end, int last_status)
{
	char	*sub;
	char	*exp;

	if (var_end > var_start)
		sub = ft_substr(arg, var_start - 1, var_end - var_start + 1);
	else
		sub = ft_strdup("$");
	exp = expand_dollar(sub, last_status);
	free(sub);
	return (exp);
}

char	*join_expanded_part(const char *arg, int *j, int *start, char *new, int last_status)
{
	char	*sub;
	char	*exp;
	int		end;

	if (*j > *start)
	{
		sub = ft_substr(arg, *start, *j - *start);
		new = ft_strjoin_free(new, sub);
	}
	(*j)++;
	end = get_var_end(arg, *j);
	exp = get_expanded_var(arg, *j, end, last_status);
	new = ft_strjoin_free(new, exp);
	*j = end;
	*start = end;
	return (new);
}

// char	*join_expanded_part(const char *arg, int *j, int *start, char *new,
// 		int last_status)
// {
// 	char	*sub;
// 	char	*exp;
// 	int		k;

// 	if (*j > *start)
// 	{
// 		sub = ft_substr(arg, *start, *j - *start);
// 		new = ft_strjoin_free(new, sub);
// 	}
// 	(*j)++;
// 	k = *j;
// 	if (arg[k] == '?')
// 		k++;
// 	else if (ft_isdigit(arg[k]))
// 		k++;
// 	else if (ft_isalnum(arg[k]) || arg[k] == '_')
// 	{
// 		k++;
// 		while (ft_isalnum(arg[k]) || arg[k] == '_')
// 			k++;
// 	}
// 	else
// 		k = *j;
// 	if (k > *j)
// 		sub = ft_substr(arg, *j - 1, k - *j + 1);
// 	else
// 		sub = ft_strdup("$");
// 	exp = expand_dollar(sub, last_status);
// 	new = ft_strjoin_free(new, exp);
// 	free(sub);
// 	*j = k;
// 	*start = k;
// 	return (new);
// }

char *expand_token(char *arg, t_data *data)
{
	int		j;
	int		start;
	char	*new;
	char	*sub;

	j = 0;
	start = 0;
	new = ft_strdup("");
	while (arg[j])
	{
		if (arg[j] == '$')
			new = join_expanded_part(arg, &j, &start, new, data->last_status);
		else
			j++;
	}
	if (j > start)
	{
		sub = ft_substr(arg, start, j - start);
		new = ft_strjoin_free(new, sub);
	}
	return (new);
}

char **expand(char **args, t_data *data)
{
	int		i;
	char	*exp;

	i = 0;
	while (args[i])
	{
		if (args[i][0] == '\'')
			args[i] = expand_single_quote(args[i]);
		else if (args[i][0] == '"')
			args[i] = expand_double_quote(args[i], data);
		else if (ft_strchr(args[i], '$'))
		{
			exp = expand_token(args[i], data);
			free(args[i]);
			args[i] = exp;
			//free(exp);
		}
		i++;
	}
	return (args);
}
