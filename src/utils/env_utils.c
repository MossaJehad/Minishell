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

static void	update_quote_state(t_parse_state *ps, char c)
{
	if (c == '\'' && !ps->in_double_quote)
		ps->in_single_quote = !ps->in_single_quote;
	else if (c == '"' && !ps->in_single_quote)
		ps->in_double_quote = !ps->in_double_quote;
}

static void	process_space(t_parse_state *ps, int *in_word)
{
	if (*in_word)
	{
		ps->arg_count++;
		*in_word = 0;
	}
}

int	count_words_with_quotes(const char *str)
{
	t_parse_state	ps;
	int				in_word;

	null_parse_state(&ps);
	in_word = 0;
	while (str[ps.cursor])
	{
		update_quote_state(&ps, str[ps.cursor]);
		if (ft_isspace(str[ps.cursor])
			&& !ps.in_single_quote && !ps.in_double_quote)
			process_space(&ps, &in_word);
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

static char	**allocate_word_array(const char *str, int word_count)
{
	char	**words;
	int		i;
	int		pos;
	char	*word;

	words = ft_calloc(word_count + 1, sizeof(char *));
	if (words == NULL)
		return (NULL);
	i = 0;
	pos = 0;
	while (i < word_count)
	{
		word = extract_word(str, &pos);
		if (word == NULL)
		{
			ft_free_arr(words);
			return (NULL);
		}
		words[i] = word;
		i++;
	}
	words[i] = NULL;
	return (words);
}

char	**split_words_with_quotes(const char *str)
{
	int		word_count;
	char	**words;

	if (str == NULL)
		return (NULL);
	if (*str == '\0')
		return (NULL);
	word_count = count_words_with_quotes(str);
	if (word_count == 0)
		return (NULL);
	words = allocate_word_array(str, word_count);
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

static int	count_words_from_split(char *arg)
{
	char	**split;
	int		count;
	int		j;

	split = split_words_with_quotes(arg);
	if (split == NULL)
		return (1);
	count = 0;
	j = 0;
	while (split[j] != NULL)
	{
		count++;
		j++;
	}
	ft_free_arr(split);
	return (count);
}

static int	count_split_args(char **args)
{
	int		total;
	int		i;
	int		count;

	total = 0;
	i = 0;
	while (args[i] != NULL)
	{
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
