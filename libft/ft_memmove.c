/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 13:04:57 by jhaddadi          #+#    #+#             */
/*   Updated: 2025/07/23 14:17:39 by mhasoneh         ###   ########.fr       */
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
