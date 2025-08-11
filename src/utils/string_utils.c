/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:28:48 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 17:48:07 by mhasoneh         ###   ########.fr       */
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

char	*ft_strndup(const char *s1, size_t n)
{
	char	*copy;
	size_t	len;
	size_t	i;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1);
	if (n < len)
		len = n;
	copy = malloc(len + 1);
	if (!copy)
		return (NULL);
	i = 0;
	while (i < len && s1[i])
	{
		copy[i] = s1[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

int	copy_char(const char *arg, int *i, char *buffer, int j)
{
	buffer[j] = arg[*i];
	(*i)++;
	return (j + 1);
}
