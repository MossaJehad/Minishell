/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 10:18:13 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 15:51:34 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	start_index(char const *s1, char const *set)
{
	int	i;
	int	j;
	int	key;
	int	start;

	i = 0;
	start = 0;
	while (s1[i])
	{
		j = 0;
		key = 0;
		while (set[j])
		{
			if (s1[i] == set[j])
				key = 1;
			j++;
		}
		if (key == 0)
			break ;
		i++;
		start++;
	}
	return (start);
}

int	end_index(char const *s1, char const *set, int start)
{
	int	i;
	int	j;
	int	key;
	int	end;

	end = ft_strlen(s1) - 1;
	i = end;
	while (i >= start)
	{
		j = 0;
		key = 0;
		while (set[j] && s1[i])
		{
			if (s1[i] == set[j])
				key = 1;
			j++;
		}
		if (key == 0)
			break ;
		i--;
		end--;
	}
	return (end);
}

char	*final_str(char const *s1, int start, int end)
{
	char	*sub;
	int		i;

	sub = (char *)malloc(end - start + 2);
	if (!sub)
	{
		ft_free((void **)&sub);
		return (NULL);
	}
	i = 0;
	while (start <= end)
	{
		sub[i] = s1[start];
		i++;
		start++;
	}
	sub[i] = '\0';
	return (sub);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int	start;
	int	end;

	if (!set)
		return ((char *)s1);
	if (!s1)
		return (NULL);
	start = start_index(s1, set);
	end = end_index(s1, set, start);
	return (final_str(s1, start, end));
}
