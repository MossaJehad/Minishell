/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_char.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:46:44 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 12:47:24 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	copy_char(const char *arg, int *i, char *buffer, int j)
{
	buffer[j++] = arg[(*i)++];
	return (j);
}
