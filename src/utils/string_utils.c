/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:28:48 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/29 17:28:59 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	has_unclosed_quotes(const char *input)
{
	int	in_single;
	int	in_double;

	in_double = 0;
	in_single = 0;
	while (*input)
	{
		if (*input == '\'' && !in_double)
			in_single = !in_single;
		else if (*input == '"' && !in_single)
			in_double = !in_double;
		input++;
	}
	return (in_single || in_double);
}

int	is_blank(const char *s)
{
	if (!s)
		return (1);
	while (*s)
	{
		if (!ft_isspace((unsigned char)*s))
			return (0);
		s++;
	}
	return (1);
}

char	*trim_whitespace(char *str)
{
	char	*start;
	char	*end;
	char	*result;
	size_t	len;

	if (!str)
		return (NULL);
	start = str;
	while (*start && ft_isspace(*start))
		start++;
	if (*start == '\0')
		return (ft_strdup(""));
	end = start + ft_strlen(start) - 1;
	while (end > start && ft_isspace(*end))
		end--;
	len = end - start + 1;
	result = ft_calloc(len + 1, sizeof(char));
	if (!result)
		return (NULL);
	ft_strncpy(result, start, len);
	result[len] = '\0';
	return (result);
}

int	is_quoted_expansion(const char *original_arg)
{
	if (!original_arg)
		return (0);
	return ((original_arg[0] == '"'
			&& original_arg[ft_strlen(original_arg) - 1] == '"')
		|| (original_arg[0] == '\''
			&& original_arg[ft_strlen(original_arg) - 1] == '\''));
}

int	count_words_from_split(char *arg)
{
	char	**split;
	int		count;
	int		j;

	split = split_words_with_quotes(arg);
	if (split == NULL)
		return (1);
	count = 0;
	j = 0;
	while (split[j] != NULL)
	{
		count++;
		j++;
	}
	ft_free_arr(split);
	return (count);
}
