/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_buf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 03:55:19 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/19 03:55:35 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_itoa_buf(int n, char *buf, int i, int j)
{
	char	tmp[12];
	int		neg;

	neg = 0;
	if (n == 0)
	{
		buf[0] = '0';
		buf[1] = '\0';
		return ;
	}
	if (n < 0)
	{
		neg = 1;
		n = -n;
	}
	while (n > 0)
	{
		tmp[i++] = (n % 10) + '0';
		n /= 10;
	}
	if (neg)
		tmp[i++] = '-';
	while (j < i)
	{
		buf[j] = tmp[i - j - 1];
		j++;
	}
	buf[i] = '\0';
}
