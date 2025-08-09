/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_arr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 03:51:14 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 03:51:35 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Free a NULL-terminated array of pointers and set the array pointer to NULL
void ft_free_arr(void ***arr)
{
	int i;

	if (!arr || !*arr)
		return;

	for (i = 0; (*arr)[i] != NULL; i++)
	{
		free((*arr)[i]);
		(*arr)[i] = NULL;
	}

	free(*arr);
	*arr = NULL;
}
