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

static int	count_split_args(char **args)
{
	int		total;
	int		j;
	char	**split;
	int		i;

	total = 0;
	i = 0;
	while (args[i])
	{
		if (!is_quoted_expansion(args[i]) && ft_strchr(args[i], ' '))
		{
			split = split_words_with_quotes(args[i]);
			if (split)
			{
				j = 0;
				while (split[j++])
					total++;
				ft_free_arr(split);
			}
			else
				total++;
		}
		else
			total++;
		i++;
	}
	return (total);
}

static int	copy_split_array_to_args(char **split, char **new_args, int *k)
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

static int	copy_and_split_arg(char *arg, char **new_args, int *k)
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

static void	fill_split_args(char **args, char **new_args)
{
	int		i;
	int		k;
	char	*dup;

	i = 0;
	k = 0;
	while (args[i])
	{
		if (!is_quoted_expansion(args[i]) && ft_strchr(args[i], ' '))
		{
			copy_and_split_arg(args[i], new_args, &k);
		}
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
	new_args = ft_calloc(new_count + 1, sizeof(char *));
	if (!new_args)
		return (args);
	fill_split_args(args, new_args);
	ft_free_arr(args);
	return (new_args);
}
