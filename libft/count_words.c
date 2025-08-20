/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 00:27:28 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/20 00:28:14 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	count_words(char *str)
{
	int	count;
	int	in_word;
	int	i;

	i = 0;
	count = 0;
	in_word = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t' && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if ((str[i] == ' ' || str[i] == '\t') && in_word)
		{
			in_word = 0;
		}
		i++;
	}
	return (count);
}
