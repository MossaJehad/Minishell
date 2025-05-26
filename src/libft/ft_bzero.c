/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 11:51:16 by jhaddadi          #+#    #+#             */
/*   Updated: 2025/05/26 14:23:45 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	char	*p;

	p = s;
	while (n > 0)
	{
		*p = 0;
		n--;
		p++;
	}
}

/*int	main (void)
{
	char	arr[15];
	int	i;

	i = 0;
	ft_bzero(arr, sizeof(arr));
	while (i < 15)
	{
		printf("%d ", arr[i]);
		i++;
	}
	return (0);
}*/
