/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhaddadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 14:03:27 by jhaddadi          #+#    #+#             */
/*   Updated: 2024/09/08 11:19:51 by jhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*arr;
	size_t	i;
	size_t	total;

	i = 0;
	total = nmemb * size;
	arr = (char *)malloc(total);
	if (!arr)
		return (NULL);
	while (i < total)
	{
		arr[i] = 0;
		i++;
	}
	return (arr);
}

/*int	main(void)
{
	int		i;
	char	*arr;

	i = 0;
	arr = ft_calloc(4, 4);
	while (i < 4 * 4)
	{
		printf("%d ", arr[i]);
		i++;
	}
}*/
