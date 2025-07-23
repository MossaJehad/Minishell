/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 14:03:27 by jhaddadi          #+#    #+#             */
/*   Updated: 2025/07/23 14:17:17 by mhasoneh         ###   ########.fr       */
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
	{
		ft_free(&arr);
		return (NULL);
	}
	while (i < total)
	{
		arr[i] = 0;
		i++;
	}
	return (arr);
}
