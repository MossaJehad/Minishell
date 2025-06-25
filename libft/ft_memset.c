/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:05:12 by jhaddadi          #+#    #+#             */
/*   Updated: 2025/05/26 14:24:18 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	char	*p;

	p = s;
	while (n > 0)
	{
		*p = (char)c;
		n--;
		p++;
	}
	return (s);
}

/*int	main (void)
{
	char    arr[15];
	int     i;

	i = 0;
	ft_memset(arr, 0, sizeof(arr));
	while (i < 15)
	{
		printf("%d ", arr[i]);
		i++;
	}
	return (0);
}*/
