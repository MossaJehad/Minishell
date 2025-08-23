/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_buf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 03:55:19 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/23 18:26:06 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	fill_digits(int n, char *tmp)
{
	int	i;
	int	neg;

	i = 0;
	neg = 0;
	if (n < 0)
	{
		neg = 1;
		if (n == -2147483648)
		{
			tmp[i++] = '8';
			n /= 10;
		}
		n = -n;
	}
	while (n > 0)
	{
		tmp[i++] = (n % 10) + '0';
		n /= 10;
	}
	if (neg)
		tmp[i++] = '-';
	return (i);
}

void	ft_itoa_buf(int n, char *buf, int i, int j)
{
	char	tmp[12];
	int		len;

	if (n == 0)
	{
		buf[0] = '0';
		buf[1] = '\0';
		return ;
	}
	len = fill_digits(n, tmp);
	while (j < len)
	{
		buf[j] = tmp[len - j - 1];
		j++;
	}
	buf[len] = '\0';
}
