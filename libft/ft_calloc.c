/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 14:03:27 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 04:19:43 by mhasoneh         ###   ########.fr       */
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
		ft_free((void **)&arr);
		return (NULL);
	}
	while (i < total)
	{
		arr[i] = 0;
		i++;
	}
	return (arr);
}
