/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 12:53:23 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/20 02:45:53 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_operator(const char *s)
{
	if (!s)
		return (0);
	return (
		ft_strcmp(s, "|")	== 0 ||
		ft_strcmp(s, "<")	== 0 ||
		ft_strcmp(s, ">")	== 0 ||
		ft_strcmp(s, ">>")	== 0 ||
		ft_strcmp(s, "<<")	== 0
	);
}

int	should_run_in_parent(const char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "exit"));
}

void	cleanup_and_exit(int exit_code)
{
	rl_clear_history();
	exit(exit_code);
}

void	cleanup_shell_resources(char ***env, t_token *token, char **args, char *input)
{
	if (token)
	{
		free_tokens(token);
		token = NULL;
	}
	if (args)
	{
		ft_free_arr(args);
		args = NULL;
	}
	if (input)
	{
		free(input);
		input = NULL;
	}
	if (env && *env)
	{
		ft_free_arr(*env);
		*env = NULL;
	}
}

void	cleanup_all(t_shell *shell)
{
	t_clean	*current;
	t_clean	*next;

	current = shell->clean;
	while (current)
	{
		next = current->next;
		if (current->ptr)
		{
			free(current->ptr);
			current->ptr = NULL;
		}
		free(current);
		current = next;
	}
	shell->clean = NULL;
}

void	*tracked_malloc(t_shell *shell, size_t size)
{
	void	*ptr;
	t_clean	*node;

	if (!shell || !size)
		return (NULL);
	ptr = malloc(size);
	if (!ptr)
		err_message(shell, "malloc", NULL, NULL);
	node = (t_clean *)malloc(sizeof(t_clean));
	if (!node)
	{
		free(ptr);
		err_message(shell, "malloc", NULL, NULL);
	}
	node->ptr = ptr;
	node->next = shell->clean;
	shell->clean = node;
	return (ptr);
}

char	*ft_strdup_track(t_shell *shell, char *src)
{
	int		srclen;
	char	*dup;
	int		i;

	if (!src)
		return (NULL);
	srclen = ft_strlen(src);
	dup = tracked_malloc(shell, sizeof(char) * (srclen + 1));
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (src[i] != '\0')
	{
		dup[i] = src[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_substr_track(t_shell *shell, char *s, unsigned int start, size_t n)
{
	char	*str;
	size_t	i;

	if (s == NULL)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup_track(shell, ""));
	if (n >= ft_strlen(s) - start)
		n = ft_strlen(s) - start;
	str = tracked_malloc(shell, sizeof(char) * (n + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (i < n)
		str[i++] = s[start++];
	str[n] = '\0';
	return (str);
}


