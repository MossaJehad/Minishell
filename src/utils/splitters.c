/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitters.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:12:52 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/30 14:18:15 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_split_args(char **args)
{
	int		total;
	int		i;
	int		count;

	total = 0;
	i = 0;
	while (args[i] != NULL)
	{
		if (ft_strcmp(args[i], "\001") == 0)
		{
			i++;
			continue ;
		}
		if (!is_quoted_expansion(args[i]) && ft_strchr(args[i], ' '))
		{
			count = count_words_from_split(args[i]);
			total += count;
		}
		else
		{
			total++;
		}
		i++;
	}
	return (total);
}

int	copy_split_array_to_args(char **split, char **new_args, int *k)
{
	int		j;
	char	*dup;

	j = 0;
	while (split[j] != NULL)
	{
		dup = ft_strdup(split[j]);
		if (dup == NULL)
		{
			ft_free_arr(split);
			return (-1);
		}
		new_args[*k] = dup;
		(*k)++;
		j++;
	}
	ft_free_arr(split);
	return (0);
}

int	copy_and_split_arg(char *arg, char **new_args, int *k)
{
	char	**split;
	char	*dup;
	int		result;

	split = split_words_with_quotes(arg);
	if (split != NULL)
	{
		result = copy_split_array_to_args(split, new_args, k);
		if (result == -1)
			return (-1);
	}
	else
	{
		dup = ft_strdup(arg);
		if (dup == NULL)
			return (-1);
		new_args[*k] = dup;
		(*k)++;
	}
	return (0);
}

void	fill_split_args(char **args, char **new_args)
{
	int		i;
	int		k;
	char	*dup;

	i = 0;
	k = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "\001") == 0)
		{
			i++;
			continue ;
		}
		if (!is_quoted_expansion(args[i]) && ft_strchr(args[i], ' '))
			copy_and_split_arg(args[i], new_args, &k);
		else
		{
			dup = ft_strdup(args[i]);
			if (dup)
			{
				new_args[k] = dup;
				k++;
			}
		}
		i++;
	}
	new_args[k] = NULL;
}

char	**apply_word_splitting(char **args, char **envp)
{
	char	**new_args;
	int		new_count;

	(void)envp;
	new_count = count_split_args(args);
	if (new_count == 0)
	{
		ft_free_arr(args);
		return (NULL);
	}
	new_args = ft_calloc(new_count + 1, sizeof(char *));
	if (!new_args)
		return (args);
	fill_split_args(args, new_args);
	ft_free_arr(args);
	return (new_args);
}
