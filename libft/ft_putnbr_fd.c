/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 10:46:31 by jhaddadi          #+#    #+#             */
/*   Updated: 2025/07/23 14:17:48 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	s[11];
	int		i;

	i = 0;
	if (n < 0)
	{
		if (n == -2147483648)
		{
			write(fd, "-2147483648", 11);
			return ;
		}
		write(fd, "-", 1);
		n = -n;
	}
	if (n == 0)
		write(fd, "0", 1);
	while (n > 0)
	{
		s[i++] = (n % 10) + '0';
		n = n / 10;
	}
	while (i > 0)
		write(fd, &s[--i], 1);
}
