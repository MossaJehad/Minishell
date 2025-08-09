/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_arr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 03:51:14 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 20:42:06 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Free a NULL-terminated array of pointers and set the array pointer to NULL
void	ft_free_arr(void ***arr)
{
	void	**array;
	int		i;

	if (!arr || !*arr)
		return;
	array = *arr;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	*arr = NULL;
}
