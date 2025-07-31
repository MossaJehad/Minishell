/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:33 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/07/31 15:13:16 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_shell_builtin(const char *cmd)
{
	if (ft_strcmp(cmd, "type") == 0 || ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "exit") == 0 || ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "unset") == 0)
	{
		return (1);
	}
	return (0);
}

int	should_run_in_parent(const char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "exit"));
}

void	print_welcome_banner(void)
{
	printf("┏┓┳┏┓┳┳┓┏┓\n");
	printf("┗┓┃┃┓┃┃┃┣┫\n");
	printf("┗┛┻┗┛┛ ┗┛┗ .\n");
	printf("			\n");
}

void	init_shell(char **envp)
{
	(void)envp;
	print_welcome_banner();
}

void	free_env(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}

char	*ft_strreplace(const char *str, const char *old, const char *new)
{
	const char	*pos;
	char		*result;
	size_t		old_len;
	size_t		new_len;
	size_t		count;
	char		*dest;

	old_len = ft_strlen(old);
	new_len = ft_strlen(new);
	count = 0;
	pos = str;
	while (1)
	{
		pos = ft_strnstr(pos, old, ft_strlen(pos));
		if (!pos)
			break ;
		count++;
		pos += old_len;
	}
	result = malloc(ft_strlen(str) + count * (new_len - old_len) + 1);
	if (!result)
		return (NULL);
	dest = result;
	while (1)
	{
		pos = ft_strnstr(str, old, ft_strlen(str));
		if (!pos)
			break ;
		ft_memcpy(dest, str, pos - str);
		dest += pos - str;
		ft_memcpy(dest, new, new_len);
		dest += new_len;
		str = pos + old_len;
	}
	ft_strlcpy(dest, str, ft_strlen(str) + 1);
	return (result);
}

char	**ft_strdup_array(char **array)
{
	int		i;
	int		count;
	char	**dup;

	count = 0;
	while (array[count])
		count++;
	dup = malloc(sizeof(char *) * (count + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < count)
	{
		dup[i] = ft_strdup(array[i]);
		i++;
	}
	dup[count] = NULL;
	return (dup);
}

void	ft_sort_array(char **array)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (array[i])
	{
		j = i + 1;
		while (array[j])
		{
			if (ft_strcmp(array[i], array[j]) > 0)
			{
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
			j++;
		}
		i++;
	}
}

void	ft_free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
