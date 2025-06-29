/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhaddadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:43:37 by jhaddadi          #+#    #+#             */
/*   Updated: 2025/06/29 14:43:51 by jhaddadi         ###   ########.fr       */
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

char	*join_expanded_part(const char *arg, int *j, int *start, char *new,
		int last_status)
{
	char	*sub;
	char	*exp;
	int		k;

	if (*j > *start)
	{
		sub = ft_substr(arg, *start, *j - *start);
		new = ft_strjoin_free(new, sub);
	}
	(*j)++;
	k = *j;
	if (arg[k] == '?')
		k++;
	else
		while (ft_isalnum(arg[k]) || arg[k] == '_')
			k++;
	sub = ft_substr(arg, *j - 1, k - *j + 1);
	exp = expand_dollar(sub, last_status);
	new = ft_strjoin_free(new, exp);
	free(sub);
	*j = k;
	*start = k;
	return (new);
}
