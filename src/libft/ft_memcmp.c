/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memcmp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhaddadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 11:15:33 by jhaddadi          #+#    #+#             */
/*   Updated: 2024/09/07 15:02:08 by jhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*st1;
	const unsigned char	*st2;

	st1 = s1;
	st2 = s2;
	while (n > 0)
	{
		if (*st1 != *st2)
			return (*st1 - *st2);
		n--;
		st1++;
		st2++;
	}
	return (0);
}

/*int	main(void)
{
	const char s1[] = "Hello";
	const char s2[] = "HellO";
	printf("%d", ft_memcmp (s1, s2, 4));
}*/
