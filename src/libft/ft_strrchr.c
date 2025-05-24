/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhaddadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 11:43:43 by jhaddadi          #+#    #+#             */
/*   Updated: 2024/09/14 16:24:58 by jhaddadi         ###   ########.fr       */
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

/*int	main (void)
{
	printf ("%s", ft_strrchr("borrow", 'r'));
}*/
