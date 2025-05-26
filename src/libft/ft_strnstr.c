/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 15:02:40 by jhaddadi          #+#    #+#             */
/*   Updated: 2025/05/26 14:24:56 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;
	char	*res;

	i = 0;
	j = 0;
	if (!little[0])
		return ((char *)big);
	while (big[i] && i < len)
	{
		j = 0;
		if (big[i] == little[j])
		{
			res = (char *)&big[i];
			while (big[i + j] == little[j] && big[i + j] && little[j] && i
				+ j < len)
				j++;
			if (!little[j])
				return (res);
		}
		i++;
	}
	return (NULL);
}

/*int	main(void)
{
	printf("%s", ft_strnstr("abcdefghijk", "cde", 7));
	return (0);
}*/
