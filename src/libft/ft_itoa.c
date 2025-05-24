/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhaddadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 10:55:50 by jhaddadi          #+#    #+#             */
/*   Updated: 2024/09/09 11:16:04 by jhaddadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static char	*ft_min(void)
{
	char	*str;

	str = (char *)malloc(12);
	if (!str)
		return (NULL);
	ft_strlcpy(str, "-2147483648", 12);
	return (str);
}

static char	*ft_final(char *temp, char *str, int i)
{
	int	j;
	int	w;

	j = 0;
	w = 0;
	if (!str)
		return (NULL);
	if (temp[0] == '-')
	{
		str[j] = '-';
		j++;
		w = 1;
	}
	while (i >= w)
	{
		str[j] = temp[i];
		j++;
		i--;
	}
	str[j] = '\0';
	return (str);
}

static char	*ft_zero_case(void)
{
	char	*str;

	str = (char *)malloc(2);
	if (!str)
		return (NULL);
	str[0] = '0';
	str[1] = '\0';
	return (str);
}

char	*ft_itoa(int n)
{
	char	temp[12];
	char	*str;
	int		i;

	i = 0;
	if (n == 0)
	{
		return (ft_zero_case());
	}
	else if (n < 0)
	{
		if (n == -2147483648)
			return (ft_min());
		temp[i++] = '-';
		n = -n;
	}
	while (n > 0)
	{
		temp[i++] = (n % 10) + '0';
		n = n / 10;
	}
	temp[i] = '\0';
	str = (char *)malloc(i + 1);
	return (ft_final(temp, str, i - 1));
}

/*int	main(void)
{
	printf("%s", ft_itoa(-34695));
}*/
