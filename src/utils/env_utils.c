/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 20:50:15 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/29 17:12:45 by mhasoneh         ###   ########.fr       */
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

void	update_quote_state(t_parse_state *ps, char c)
{
	if (c == '\'' && !ps->in_double_quote)
		ps->in_single_quote = !ps->in_single_quote;
	else if (c == '"' && !ps->in_single_quote)
		ps->in_double_quote = !ps->in_double_quote;
}

void	process_space(t_parse_state *ps, int *in_word)
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
