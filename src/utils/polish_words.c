/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polish_words.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:12:06 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/29 17:28:49 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	process_char(const char *str, int *pos,
	char buffer[BUFFER_SIZE], int *state)
{
	int	buf_idx;

	buf_idx = 0;
	if (str[*pos] == '\'' && !(state[1]))
	{
		state[0] = !state[0];
		buffer[buf_idx++] = str[(*pos)++];
	}
	else if (str[*pos] == '"' && !(state[0]))
	{
		state[1] = !state[1];
		buffer[buf_idx++] = str[(*pos)++];
	}
	else if (ft_isspace(str[*pos]) && !state[0] && !state[1])
		return (-1);
	else
		buffer[buf_idx++] = str[(*pos)++];
	return (buf_idx);
}

char	*build_word(const char *str, int *pos)
{
	char	buffer[BUFFER_SIZE];
	int		buf_idx;
	int		state[2];
	int		ret;

	buf_idx = 0;
	state[0] = 0;
	state[1] = 0;
	while (str[*pos] && buf_idx < BUFFER_SIZE - 1)
	{
		ret = process_char(str, pos, buffer + buf_idx, state);
		if (ret == -1)
			break ;
		buf_idx += ret;
	}
	buffer[buf_idx] = '\0';
	if (buf_idx == 0)
		return (NULL);
	return (ft_strdup(buffer));
}

char	*extract_word(const char *str, int *pos)
{
	while (str[*pos] && ft_isspace(str[*pos]))
		(*pos)++;
	return (build_word(str, pos));
}

char	**allocate_word_array(const char *str, int word_count)
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
