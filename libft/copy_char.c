/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_char.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:46:44 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/09 20:16:38 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	copy_char(const char *arg, int *i, char *buffer, int j)
{
	if (arg && buffer)
		buffer[j] = arg[*i];
	(*i)++;
	return (j + 1);
}
