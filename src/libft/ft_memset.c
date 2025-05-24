/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhaddadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:05:12 by jhaddadi          #+#    #+#             */
/*   Updated: 2024/09/07 14:13:29 by jhaddadi         ###   ########.fr       */
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
