/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup_array.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:51:43 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 12:52:01 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_strdup_array(char **array)
{
	int		i;
	int		count;
	char	**dup;

	count = 0;
	while (array[count])
		count++;
	dup = malloc(sizeof(char *) * (count + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < count)
	{
		dup[i] = ft_strdup(array[i]);
		i++;
	}
	dup[count] = NULL;
	return (dup);
}
