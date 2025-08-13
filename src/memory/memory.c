/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 00:00:00 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 05:12:04 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		perror("minishell: malloc");
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

char	*safe_strdup(const char *s)
{
	char	*dup;

	if (!s)
		return (NULL);
	
	dup = ft_strdup(s);
	if (!dup)
	{
		perror("minishell: strdup");
		exit(EXIT_FAILURE);
	}
	return (dup);
}

char	**safe_split(char const *s, char c)
{
	char	**result;

	if (!s)
		return (NULL);
	
	result = ft_split(s, c);
	if (!result)
	{
		perror("minishell: split");
		exit(EXIT_FAILURE);
	}
	return (result);
}

void	free_string_array(char **arr)
{
	int	i;

	if (!arr)
		return;
	
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	cleanup_and_exit(t_shell *shell, int exit_code)
{
	cleanup_shell(shell);
	exit(exit_code);
}

/* 
** Safe memory allocation with automatic cleanup on failure
** This ensures we don't have partial allocations that leak memory
*/
char	**allocate_string_array(int size)
{
	char	**arr;

	arr = (char **)safe_malloc(sizeof(char *) * (size + 1));
	arr[size] = NULL;
	return (arr);
}

/*
** Reallocate string array with proper cleanup
*/
//char	**realloc_string_array(char **old_arr, int old_size, int new_size)
//{
//	char	**new_arr;
//	int		i;

//	new_arr = allocate_string_array(new_size);
	
//	i = 0;
//	while (i < old_size && old_arr && old_arr[i])
//	{
//		new_arr[i] = safe_strdup(old_arr[i]);
//		i++;
//	}
	
//	if (old_arr)
//		free_string_array(old_arr);
	
//	return (new_arr);
//}

/*
** Join two strings and free the first one
** Useful for building strings incrementally without leaks
*/
char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*result;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (safe_strdup(s2));
	if (!s2)
		return (s1);
	
	result = ft_strjoin(s1, s2);
	free(s1);
	
	if (!result)
	{
		perror("minishell: strjoin");
		exit(EXIT_FAILURE);
	}
	return (result);
}