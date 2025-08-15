/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 12:08:29 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/15 17:31:30 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	count_word(char const *s, char c)
{
	int	i;
	int	count;

	if (!s)
		return (0);
	i = 0;
	count = 0;
	while (s[i] == c && s[i])
		i++;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

char	*ft_word(char const *s, int start, int end)
{
	char	*word;
	int		i;

	i = 0;
	if (start > end)
		return (NULL);
	word = malloc((end - start + 2) * sizeof(char));
	if (!word)
		return (NULL);
	while (start <= end)
	{
		word[i] = s[start];
		start++;
		i++;
	}
	word[i] = '\0';
	return (word);
}

void	ft_free2(char **arr, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	**ft_split_internal(char const *s, char c, char **arr, int j)
{
	int	i;
	int	start;

	i = 0;
	while (s[j] == c && s[j])
		j++;
	start = j;
	while (s[j])
	{
		if (s[j] != c && start <= j && (s[j + 1] == c || s[j + 1] == '\0'))
		{
			arr[i] = ft_word(s, start, j);
			if (!arr[i])
			{
				ft_free2(arr, i);
				return (NULL);
			}
			i++;
		}
		if (s[j] == c && s[j + 1] != c)
			start = j + 1;
		j++;
	}
	arr[i] = NULL;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	int		j;

	j = 0;
	if (!s)
		return (NULL);
	arr = (char **)malloc((count_word(s, c) + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	while (s[j] == c && s[j])
		j++;
	arr = ft_split_internal(s, c, arr, j);
	return (arr);
}
