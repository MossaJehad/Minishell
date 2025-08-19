/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_char.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 12:46:44 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/17 21:23:58 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	copy_char(const char *input, int *in_idx, char *output, int out_idx)
{
	if (out_idx < BUFFER_SIZE * 4 - 1)
		output[out_idx++] = input[(*in_idx)++];
	else
		(*in_idx)++;
	return (out_idx);
}
