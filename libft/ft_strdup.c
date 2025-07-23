/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 12:13:35 by jhaddadi          #+#    #+#             */
/*   Updated: 2025/07/23 14:16:28 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strdup(const char *s)
{
	if (!s)
		return NULL;
	size_t len = ft_strlen(s);
	char *copy = malloc(len + 1);
	if (!copy)
		return NULL;
	ft_memcpy(copy, s, len + 1);
	return copy;
}
