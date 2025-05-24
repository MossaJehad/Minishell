/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhaddadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 13:04:57 by jhaddadi          #+#    #+#             */
/*   Updated: 2024/09/09 10:17:47 by jhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*c_src;
	unsigned char	*c_dst;

	if (!dst && !src)
		return (dst);
	c_src = (unsigned char *)src;
	c_dst = (unsigned char *)dst;
	if (c_dst > c_src)
	{
		c_dst += len;
		c_src += len;
		while (len-- > 0)
			*--c_dst = *--c_src;
	}
	else
	{
		while (len-- > 0)
			*c_dst++ = *c_src++;
	}
	return (dst);
}

/*int	main(void)
{
	char dst[] = "0123456789";
	const char src[] = "hello world";
	ft_memmove (dst, src, 5);
	printf("%s", dst);
	return (0);
}*/
