/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:30:50 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/23 18:47:40 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*
		Under Construction
*/
char	*ft_strtok(char *str, const char *delim)
{
	(void)str;
	(void)delim;
	return (NULL);
}

char	*path_join(const char *dir, const char *file)
{
	char	*result;

	result = malloc(ft_strlen(dir) + ft_strlen(file) + 2);
	return (result);
}
