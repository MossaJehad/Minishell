/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:20:35 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/05/27 16:59:44 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// #include "libft.h"
////// LATEST

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

// Copies characters from src to buffer
// interpreting escape sequences if inside a quoted string.
int	whileloopstring(int i, int j, int len, char *buffer,
						const char *src, int bufsize, int string)
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

char	*print_inside_quotes(const char *src)
{
	char	quote_char;
	char	*string;
	int		i;
	int		j;

	i = 1;
	j = 0;
	if (!src || (src[0] != '\'' && src[0] != '"'))
		return (strdup(src));
	quote_char = src[0];
	string = malloc(strlen(src));
	if (!string)
		return (NULL);
	while (src[i] && src[i] != quote_char)
		string[j++] = src[i++];
	string[j] = '\0';
	return (string);
}

void	nully(t_parse_state *s)
{
	s->i = 0;
	s->j = 0;
	s->k = 0;
	s->in_single_quote = 0;
	s->in_double_quote = 0;
}

// Splits the input line into arguments
// while respecting quotes and escape characters.
char	**parse_arguments(const char *input, int *arg_count, int *quote_error)
{
	t_parse_state	s;
	static char		*argv[MAX_ARGS];
	char			buffer[BUFFER_SIZE];
	char	next;

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
		if (!s.in_single_quote && !s.in_double_quote
			&& (input[s.i] == ' ' || input[s.i] == '\t'))
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
		if ((s.in_single_quote || s.in_double_quote)
			&& (input[s.i] == '\\' && input[s.i + 1] == 'n'))
		{
			buffer[s.j++] = input[s.i++];
			continue ;
		}
		if (input[s.i] == '\\' && input[s.i + 1])
		{
			next = input[s.i + 1];
			if (!s.in_single_quote && !s.in_double_quote)
			{
				// Outside quotes: drop the backslash, keep character
				buffer[s.j++] = next;
				s.i += 2;
				continue ;
			}
			else
			{
				// Inside quotes: preserve both
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
	*quote_error = s.in_single_quote || s.in_double_quote;
	return (argv);
}

// void print_tokens(t_token *token)
// {
//     t_token *temp;

//     while (token)
//     {
//         printf("type: %s value: %s\n", token->type, token->value);
//         temp = token;
//         token = token->next;
//         free(temp->value);
//         free(temp->type);
//         free(temp);
//     }
// }
// Runs the main shell loop that reads user input,
// parses it, and executes commands.
void	shell_loop(int arg_count, t_data *data)
{
	char		*input;
	char		**args;
	int			quote_error;
	t_token		*token;

	quote_error = 0;
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
		args = parse_arguments(input, &arg_count, &quote_error);
		if (quote_error)
		{
			printf("Unmatched quote detected!\n");
			free(input);
			continue ;
		}
		if(check_syntax_error(args, data))
		{
			free(input);
			continue;
		}
		args = expand(args, data);
		tokenize(args, &token);
		// print_tokens(token);
		handle_command(input, args, arg_count, token, data);
		ft_free(args);
		free_tokens(token);
	}
}

char	**copy_env(char **envp)
{
	int		i = 0;
	char	**new_env;

	while (envp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}
int	main(int argc, char **argv, char **envp)
{
	t_data data;
	
	(void)argc;
	(void)argv;
	data.env = copy_env(envp);
	data.last_status = 0;
	init_shell();
	shell_loop(argc, &data);
	rl_clear_history();
	free_env(data.env);
	return (data.last_status);
}

