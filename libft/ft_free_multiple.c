/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_multiple.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 14:52:53 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/22 00:17:56 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Free a multiple pointer and set it to NULL
void	ft_free_multiple(void **ptrs[], size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		if (ptrs[i] && *ptrs[i])
		{
			free(*ptrs[i]);
			*ptrs[i] = NULL;
		}
		i++;
	}
}
