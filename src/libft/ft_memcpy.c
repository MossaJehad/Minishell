/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhaddadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 12:22:46 by jhaddadi          #+#    #+#             */
/*   Updated: 2024/09/08 14:58:38 by jhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char		*d;
	const char	*s;

	d = dest;
	s = src;
	if (dest == NULL && src == NULL)
		return (dest);
	while (n > 0)
	{
		*d = (char)*s;
		n--;
		d++;
		s++;
	}
	return (dest);
}

/*int	main(void)
{
	char	dest[] = "0123456789";
	const char	src[] = "hello world";
	ft_memcpy(dest, src, 5);
	printf("%s", dest);
	return (0);
}*/
