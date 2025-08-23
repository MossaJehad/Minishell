/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:30:50 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/10 14:20:12 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtok(char *str, const char *delim)
{
	char	*save;
	char	*token;
	int		i;

	i = 0;
	if (str)
		save = str;
	else
		save = NULL;
	if (!save || *save == '\0')
		return (NULL);
	while (*save && ft_strchr(delim, *save))
		save++;
	if (*save == '\0')
		return (NULL);
	token = save;
	while (save[i] && !ft_strchr(delim, save[i]))
		i++;
	if (save[i])
	{
		save[i] = '\0';
		save = &save[i + 1];
	}
	else
		save = NULL;
	return (token);
}

char	*path_join(const char *dir, const char *file)
{
	char	*result;

	result = malloc(ft_strlen(dir) + ft_strlen(file) + 2);
	sprintf(result, "%s/%s", dir, file);
	return (result);
}
