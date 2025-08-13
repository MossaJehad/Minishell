/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:28:05 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 05:29:54 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*lookup_env_value(const char *name, char **envp)
{
	size_t	len;
	int		i;

	if (!name || !envp)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], name, len) && envp[i][len] == '=')
			return (&envp[i][len + 1]);
		i++;
	}
	return (NULL);
}

//char	**ft_strdup_array(char **array)
//{
//	int		i;
//	int		count;
//	char	**dup;

//	if (!array)
//		return (NULL);
//	count = 0;
//	while (array[count])
//		count++;
//	dup = malloc(sizeof(char *) * (count + 1));
//	if (!dup)
//		return (NULL);
//	i = 0;
//	while (i < count)
//	{
//		dup[i] = ft_strdup(array[i]);
//		if (!dup[i])
//		{
//			while (--i >= 0)
//				free(dup[i]);
//			free(dup);
//			return (NULL);
//		}
//		i++;
//	}
//	dup[count] = NULL;
//	return (dup);
//}

//void	ft_sort_array(char **array)
//{
//	int		i;
//	int		j;
//	char	*temp;

//	if (!array)
//		return ;
//	i = 0;
//	while (array[i])
//	{
//		j = i + 1;
//		while (array[j])
//		{
//			if (ft_strcmp(array[i], array[j]) > 0)
//			{
//				temp = array[i];
//				array[i] = array[j];
//				array[j] = temp;
//			}
//			j++;
//		}
//		i++;
//	}
//}
