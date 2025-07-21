/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:30:04 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/07/21 18:49:39 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

void	check_inloop(int j, char next, char *buffer)
{
	if (next == 'n')
		buffer[j++] = '\n';
	else if (next == 't')
		buffer[j++] = '\t';
	else if (next == '\\')
		buffer[j++] = '\\';
	else if (next == '"')
		buffer[j++] = '"';
	else if (next == '\'')
		buffer[j++] = '\'';
	else
		buffer[j++] = next;
}

int	whileloopstring(int i, int j, int len, char *buffer, const char *src,
		int bufsize, int string)
{
	char	next;

	while (i < len && j < bufsize - 1)
	{
		if (src[i] == '\\' && i + 1 < len)
		{
			next = src[i + 1];
			if (string)
				check_inloop(j, next, buffer);
			else
			{
				buffer[j++] = src[i];
				buffer[j++] = next;
			}
			i += 2;
		}
		else
			buffer[j++] = src[i++];
	}
	buffer[j] = '\0';
	return (j);
}

char	*ft_strndup(const char *s1, size_t n)
{
	char	*copy;
	size_t	len;

	len = ft_strlen(s1);
	if (n < len)
		len = n;
	copy = malloc(len + 1);
	if (!copy)
		return (NULL);
	ft_memcpy(copy, s1, len);
	copy[len] = '\0';
	return (copy);
}

char	*print_inside_quotes(const char *src)
{
	if (!src)
		return (NULL);
	if (src[0] != '\'' && src[0] != '"')
		return (ft_strdup(src));
	return (ft_strdup(src));
}

int	has_unclosed_quotes(const char *input)
{
	int	in_single;
	int	in_double;

	in_double = 0;
	in_single = 0;
	while (*input)
	{
		if (*input == '\'' && !in_double)
			in_single = !in_single;
		else if (*input == '"' && !in_single)
			in_double = !in_double;
		input++;
	}
	return (in_single || in_double);
}

void	nully(t_parse_state *s)
{
	s->i = 0;
	s->j = 0;
	s->k = 0;
	s->in_single_quote = 0;
	s->in_double_quote = 0;
}

char	**parse_arguments(const char *input, int *arg_count)
{
	t_parse_state	s;
	static char		*argv[MAX_ARGS];
	char			buffer[BUFFER_SIZE];
	char			next;

	nully(&s);
	while (input[s.i])
	{
		if (input[s.i] == '\'' && !s.in_double_quote)
		{
			s.in_single_quote = !s.in_single_quote;
			buffer[s.j++] = input[s.i++];
			continue ;
		}
		if (input[s.i] == '"' && !s.in_single_quote)
		{
			s.in_double_quote = !s.in_double_quote;
			buffer[s.j++] = input[s.i++];
			continue ;
		}
		if (!s.in_single_quote && !s.in_double_quote && (input[s.i] == ' '
				|| input[s.i] == '\t'))
		{
			if (s.j > 0)
			{
				buffer[s.j] = '\0';
				argv[s.k++] = ft_strdup(buffer);
				s.j = 0;
			}
			s.i++;
			continue ;
		}
		if ((s.in_single_quote || s.in_double_quote) && (input[s.i] == '\\'
				&& input[s.i + 1] == 'n'))
		{
			buffer[s.j++] = input[s.i++];
			continue ;
		}
		if (input[s.i] == '\\' && input[s.i + 1])
		{
			next = input[s.i + 1];
			if (!s.in_single_quote && !s.in_double_quote)
			{
				buffer[s.j++] = next;
				s.i += 2;
				continue ;
			}
			else
			{
				buffer[s.j++] = input[s.i++];
				buffer[s.j++] = input[s.i++];
				continue ;
			}
		}
		buffer[s.j++] = input[s.i++];
	}
	if (s.j > 0)
	{
		buffer[s.j] = '\0';
		argv[s.k++] = ft_strdup(buffer);
	}
	argv[s.k] = NULL;
	*arg_count = s.k;
	return (argv);
}

void	shell_loop(int arg_count, char ***envp)
{
	char	*input;
	char	**args;
	t_token	*token;

	while (1)
	{
		token = NULL;
		input = get_input();
		if (input == NULL)
			break ;
		if (*input == '\0')
		{
			free(input);
			continue ;
		}
		args = parse_arguments(input, &arg_count);
		args = expand(args, *envp);
		tokenize(args, &token);
		handle_command(input, args, arg_count, token, envp);
		ft_free(args);
		free_tokens(token);
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	**env;

	(void)argv;
	init_shell(envp);
	env = envp;
	shell_loop(argc, &env);
	rl_clear_history();
	free_env(env);
	return (0);
}
