/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhaddadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 10:43:30 by jhaddadi          #+#    #+#             */
/*   Updated: 2024/09/08 14:23:52 by jhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	char	*sub;

	i = 0;
	sub = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (sub == NULL)
		return (NULL);
	while (s1[i])
	{
		sub[i] = s1[i];
		i++;
	}
	i = 0;
	while (s2[i])
	{
		sub[ft_strlen(s1) + i] = s2[i];
		i++;
	}
	sub[ft_strlen(s1) + i] = '\0';
	return (sub);
}
