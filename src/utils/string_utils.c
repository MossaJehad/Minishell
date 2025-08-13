/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:28:48 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 05:18:34 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//int	has_unclosed_quotes(const char *input)
//{
//	int		in_single_quotes;
//	int		in_double_quotes;
//	int		i;

//	if (!input)
//		return (0);

//	in_single_quotes = 0;
//	in_double_quotes = 0;
//	i = 0;

//	while (input[i])
//	{
//		if (input[i] == '\'' && !in_double_quotes)
//			in_single_quotes = !in_single_quotes;
//		else if (input[i] == '"' && !in_single_quotes)
//			in_double_quotes = !in_double_quotes;
//		i++;
//	}

//	return (in_single_quotes || in_double_quotes);
//}

//void	print_tokens(t_token *tokens)
//{
//	t_token		*current;
//	const char	*type_names[] = {
//		"WORD", "COMMAND", "PIPE", "REDIRECT_IN", 
//		"REDIRECT_OUT", "REDIRECT_APPEND", "HEREDOC", "EOF"
//	};

//	current = tokens;
//	printf("Tokens:\n");
//	while (current)
//	{
//		printf("  [%s]: '%s'\n", type_names[current->type], 
//			current->value ? current->value : "(null)");
//		current = current->next;
//	}
//}

//char	*ft_strndup(const char *s1, size_t n)
//{
//	char	*copy;
//	size_t	len;
//	size_t	i;

//	if (!s1)
//		return (NULL);
//	len = ft_strlen(s1);
//	if (n < len)
//		len = n;
//	copy = malloc(len + 1);
//	if (!copy)
//		return (NULL);
//	i = 0;
//	while (i < len && s1[i])
//	{
//		copy[i] = s1[i];
//		i++;
//	}
//	copy[i] = '\0';
//	return (copy);
//}

//void	free_split(char **split)
//{
//	int	i;

//	if (!split)
//		return ;
//	i = 0;
//	while (split[i])
//	{
//		free(split[i]);
//		i++;
//	}
//	free(split);
//}

//int	copy_char(const char *arg, int *i, char *buffer, int j)
//{
//	buffer[j] = arg[*i];
//	(*i)++;
//	return (j + 1);
//}

char	*get_prompt(t_shell *shell)
{
	char	*pwd;
	char	*prompt;
	char	*temp;

	pwd = env_get(shell, "PWD");
	if (pwd)
	{
		temp = ft_strjoin("\033[1;32m", pwd);
		prompt = ft_strjoin(temp, "\033[0m$ ");
		free(temp);
	}
	else
	{
		prompt = ft_strdup("minishell$ ");
	}

	return (prompt);
}
int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);

	/* First character must be letter or underscore */
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);

	/* Remaining characters must be alphanumeric or underscore */
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}

	return (1);
}

void	print_error(const char *cmd, const char *arg, const char *msg)
{
	if (cmd)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((char *)cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	
	if (arg)
	{
		ft_putstr_fd((char *)arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	
	if (msg)
		ft_putstr_fd((char *)msg, STDERR_FILENO);
	
	ft_putstr_fd("\n", STDERR_FILENO);
}

int	count_commands(t_cmd *commands)
{
	int		count;
	t_cmd	*current;

	count = 0;
	current = commands;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}
