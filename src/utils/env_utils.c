/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 20:50:15 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/28 18:41:13 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	copy_env(char **dest, char **src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = ft_strdup(src[i]);
		i++;
	}
	dest[i] = NULL;
}

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

int	count_words_with_quotes(const char *str)
{
	t_parse_state	ps;
	int				in_word;

	null_parse_state(&ps);
	in_word = 0;
	while (str[ps.cursor])
	{
		if (str[ps.cursor] == '\'' && !ps.in_double_quote)
			ps.in_single_quote = !ps.in_single_quote;
		else if (str[ps.cursor] == '"' && !ps.in_single_quote)
			ps.in_double_quote = !ps.in_double_quote;
		else if (ft_isspace(str[ps.cursor])
			&& !ps.in_single_quote && !ps.in_double_quote)
		{
			if (in_word)
			{
				ps.arg_count++;
				in_word = 0;
			}
		}
		else if (!in_word)
			in_word = 1;
		ps.cursor++;
	}
	if (in_word)
		ps.arg_count++;
	return (ps.arg_count);
}

char	*extract_word(const char *str, int *pos)
{
	char	buffer[BUFFER_SIZE];
	int		buf_idx;
	int		in_single_quote;
	int		in_double_quote;

	buf_idx = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (str[*pos] && ft_isspace(str[*pos]))
		(*pos)++;
	while (str[*pos] && buf_idx < BUFFER_SIZE - 1)
	{
		if (str[*pos] == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			buffer[buf_idx++] = str[(*pos)++];
		}
		else if (str[*pos] == '"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			buffer[buf_idx++] = str[(*pos)++];
		}
		else if (ft_isspace(str[*pos]) && !in_single_quote && !in_double_quote)
			break ;
		else
			buffer[buf_idx++] = str[(*pos)++];
	}
	buffer[buf_idx] = '\0';
	if (buf_idx == 0)
		return (NULL);
	return (ft_strdup(buffer));
}

char	**split_words_with_quotes(const char *str)
{
	char	**words;
	int		word_count;
	int		pos;
	int		i;

	if (!str || !*str)
		return (NULL);
	word_count = count_words_with_quotes(str);
	if (word_count == 0)
		return (NULL);
	words = ft_calloc(word_count + 1, sizeof(char *));
	if (!words)
		return (NULL);
	pos = 0;
	i = 0;
	while (i < word_count)
	{
		words[i] = extract_word(str, &pos);
		if (!words[i])
		{
			ft_free_arr(words);
			return (NULL);
		}
		i++;
	}
	words[i] = NULL;
	return (words);
}

int	is_quoted_expansion(const char *original_arg)
{
	if (!original_arg)
		return (0);
	return ((original_arg[0] == '"'
			&& original_arg[ft_strlen(original_arg) - 1] == '"')
		|| (original_arg[0] == '\''
			&& original_arg[ft_strlen(original_arg) - 1] == '\''));
}

char	**apply_word_splitting(char **args, char **envp)
{
	char	**new_args;
	char	**split_words;
	int		new_count;
	int		i;
	int		j;
	int		k;

	(void)envp;
	new_count = 0;
	i = 0;
	while (args[i])
	{
		if (!is_quoted_expansion(args[i]) && ft_strchr(args[i], ' '))
		{
			split_words = split_words_with_quotes(args[i]);
			if (split_words)
			{
				j = 0;
				while (split_words[j])
				{
					new_count++;
					j++;
				}
				ft_free_arr(split_words);
			}
			else
				new_count++;
		}
		else
			new_count++;
		i++;
	}
	new_args = ft_calloc(new_count + 1, sizeof(char *));
	if (!new_args)
		return (args);
	i = 0;
	k = 0;
	while (args[i])
	{
		if (!is_quoted_expansion(args[i]) && ft_strchr(args[i], ' '))
		{
			split_words = split_words_with_quotes(args[i]);
			if (split_words)
			{
				j = 0;
				while (split_words[j])
				{
					new_args[k++] = ft_strdup(split_words[j]);
					j++;
				}
				ft_free_arr(split_words);
			}
			else
				new_args[k++] = ft_strdup(args[i]);
		}
		else
			new_args[k++] = ft_strdup(args[i]);
		i++;
	}
	new_args[k] = NULL;
	ft_free_arr(args);
	return (new_args);
}
