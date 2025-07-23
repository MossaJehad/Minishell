/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 11:43:43 by jhaddadi          #+#    #+#             */
/*   Updated: 2025/07/23 14:18:26 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	len;

	len = ft_strlen(s);
	s = s + len;
	if ((unsigned char)c == '\0')
		return ((char *)s);
	while (len > 0)
	{
		if (*s == (unsigned char)c)
			return ((char *)s);
		s--;
		len--;
	}
	if (*s == (unsigned char)c)
		return ((char *)s);
	return (NULL);
}
