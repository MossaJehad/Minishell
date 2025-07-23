/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 12:22:46 by jhaddadi          #+#    #+#             */
/*   Updated: 2025/07/23 14:21:25 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;
	
	i = 0;
	if (!dest && !src)
		return NULL;
	unsigned char *d = (unsigned char *)dest;
	const unsigned char *s = (const unsigned char *)src;

	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return dest;
}
