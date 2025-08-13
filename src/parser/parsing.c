/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 19:09:28 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 05:13:33 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	run_parsing_loop(const char *input, t_parse_state *s, char **argv,
		char *buffer)
{
	while (input[s->i])
	{
		if (handle_quotes(input, s, buffer))
			continue ;
		if (handle_whitespace(input, s, argv, buffer))
			continue ;
		if (handle_double_char_operators(input, s, argv, buffer))
			continue ;
		if (handle_single_char_operators(input, s, argv, buffer))
			continue ;
		if (handle_escape_sequences(input, s, buffer))
			continue ;
		buffer[s->j++] = input[s->i++];
	}
}

//char	**parse_arguments(const char *input, int *arg_count)
//{
//	t_parse_state	s;
//	char			**argv;
//	char			buffer[BUFFER_SIZE];
//	int				cleanup_i;

//	if (!input || !arg_count)
//		return (NULL);
//	argv = ft_calloc(MAX_ARGS, sizeof(char *));
//	if (!argv)
//		return (NULL);
//	null_parse_state(&s);
//	run_parsing_loop(input, &s, argv, buffer);
//	if (s.j > 0)
//	{
//		buffer[s.j] = '\0';
//		argv[s.k] = ft_strdup(buffer);
//		if (!argv[s.k])
//		{
//			cleanup_i = 0;
//			while (cleanup_i < s.k)
//			{
//				if (argv[cleanup_i])
//					free(argv[cleanup_i]);
//				cleanup_i++;
//			}
//			free(argv);
//			return (NULL);
//		}
//		s.k++;
//	}
//	argv[s.k] = NULL;
//	*arg_count = s.k;
//	return (argv);
//}

t_token	*token_new(const char *value, t_token_type type)
{
	t_token	*token;

	token = (t_token *)safe_malloc(sizeof(t_token));
	token->value = value ? safe_strdup(value) : NULL;
	token->type = type;
	token->next = NULL;
	return (token);
}

void	token_free(t_token *token)
{
	if (token)
	{
		free(token->value);
		free(token);
	}
}

void	tokens_free(t_token *tokens)
{
	t_token	*next;

	while (tokens)
	{
		next = tokens->next;
		token_free(tokens);
		tokens = next;
	}
}

void	token_add_back(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!new_token)
		return;

	if (!*tokens)
	{
		*tokens = new_token;
		return;
	}

	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}


int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ' ' || c == '\t'
		|| c == '\n' || c == '\0');
}

int	is_operator(const char *str)
{
	if (!str)
		return (0);
	
	if (ft_strncmp(str, "<<", 2) == 0)
		return (2);
	if (ft_strncmp(str, ">>", 2) == 0)
		return (2);
	if (*str == '|' || *str == '<' || *str == '>')
		return (1);
	
	return (0);
}

char	*extract_word(const char *input, int *pos)
{
	int		start;
	int		end;
	char	quote;
	int		in_quotes;

	start = *pos;
	end = start;
	in_quotes = 0;
	quote = 0;

	while (input[end])
	{
		if (!in_quotes && (input[end] == '\'' || input[end] == '"'))
		{
			quote = input[end];
			in_quotes = 1;
		}
		else if (in_quotes && input[end] == quote)
		{
			in_quotes = 0;
			quote = 0;
		}
		else if (!in_quotes && is_special_char(input[end]))
		{
			break;
		}
		end++;
	}

	*pos = end;
	return (ft_strndup(input + start, end - start));
}

void	skip_whitespace(const char *input, int *pos)
{
	while (input[*pos] && (input[*pos] == ' ' || input[*pos] == '\t'))
		(*pos)++;
}

t_token	*tokenize_operator(const char *input, int *pos)
{
	int		op_len;
	char	*op_str;
	t_token	*token;

	op_len = is_operator(input + *pos);
	if (!op_len)
		return (NULL);

	op_str = ft_strndup(input + *pos, op_len);
	*pos += op_len;

	if (ft_strcmp(op_str, "|") == 0)
		token = token_new(op_str, TOKEN_PIPE);
	else if (ft_strcmp(op_str, "<") == 0)
		token = token_new(op_str, TOKEN_REDIRECT_IN);
	else if (ft_strcmp(op_str, ">") == 0)
		token = token_new(op_str, TOKEN_REDIRECT_OUT);
	else if (ft_strcmp(op_str, ">>") == 0)
		token = token_new(op_str, TOKEN_REDIRECT_APPEND);
	else if (ft_strcmp(op_str, "<<") == 0)
		token = token_new(op_str, TOKEN_HEREDOC);
	else
		token = token_new(op_str, TOKEN_WORD);

	free(op_str);
	return (token);
}

int	is_builtin_command(const char *word)
{
	if (!word)
		return (0);
	
	return (ft_strcmp(word, "echo") == 0 || ft_strcmp(word, "cd") == 0 ||
		ft_strcmp(word, "pwd") == 0 || ft_strcmp(word, "export") == 0 ||
		ft_strcmp(word, "unset") == 0 || ft_strcmp(word, "env") == 0 ||
		ft_strcmp(word, "exit") == 0);
}

t_token_type	determine_word_type(const char *word, int is_first_word)
{
	if (is_first_word && is_builtin_command(word))
		return (TOKEN_COMMAND);
	return (TOKEN_WORD);
}

t_cmd	*cmd_new(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)safe_malloc(sizeof(t_cmd));
	cmd->args = NULL;
	cmd->argc = 0;
	cmd->input_fd = STDIN_FILENO;
	cmd->output_fd = STDOUT_FILENO;
	cmd->next = NULL;
	return (cmd);
}

void	cmd_free(t_cmd *cmd)
{
	if (cmd)
	{
		if (cmd->args)
			free_string_array(cmd->args);
		if (cmd->input_fd != STDIN_FILENO)
			close(cmd->input_fd);
		if (cmd->output_fd != STDOUT_FILENO)
			close(cmd->output_fd);
		free(cmd);
	}
}

void	commands_free(t_cmd *commands)
{
	t_cmd	*next;

	while (commands)
	{
		next = commands->next;
		cmd_free(commands);
		commands = next;
	}
}

void	cmd_add_back(t_cmd **commands, t_cmd *new_cmd)
{
	t_cmd	*current;

	if (!new_cmd)
		return;

	if (!*commands)
	{
		*commands = new_cmd;
		return;
	}

	current = *commands;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

int	count_args(t_token *start, t_token *end)
{
	t_token	*current;
	int		count;

	count = 0;
	current = start;
	while (current && current != end)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_COMMAND)
			count++;
		current = current->next;
	}
	return (count);
}

void	fill_args(t_cmd *cmd, t_token *start, t_token *end)
{
	t_token	*current;
	int		i;

	cmd->argc = count_args(start, end);
	if (cmd->argc == 0)
		return;

	cmd->args = allocate_string_array(cmd->argc);
	
	current = start;
	i = 0;
	while (current && current != end && i < cmd->argc)
	{
		if (current->type == TOKEN_WORD || current->type == TOKEN_COMMAND)
		{
			cmd->args[i] = safe_strdup(current->value);
			i++;
		}
		current = current->next;
	}
}

int	setup_redirections(t_cmd *cmd, t_token *start, t_token *end)
{
	t_token	*current;
	int		fd;

	current = start;
	while (current && current != end)
	{
		if (current->type == TOKEN_REDIRECT_IN)
		{
			if (!current->next || !current->next->value)
			{
				print_error("minishell", NULL, "syntax error near '<'");
				return (0);
			}
			fd = open(current->next->value, O_RDONLY);
			if (fd == -1)
			{
				perror(current->next->value);
				return (0);
			}
			if (cmd->input_fd != STDIN_FILENO)
				close(cmd->input_fd);
			cmd->input_fd = fd;
			current = current->next;
		}
		else if (current->type == TOKEN_REDIRECT_OUT)
		{
			if (!current->next || !current->next->value)
			{
				print_error("minishell", NULL, "syntax error near '>'");
				return (0);
			}
			fd = open(current->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror(current->next->value);
				return (0);
			}
			if (cmd->output_fd != STDOUT_FILENO)
				close(cmd->output_fd);
			cmd->output_fd = fd;
			current = current->next;
		}
		else if (current->type == TOKEN_REDIRECT_APPEND)
		{
			if (!current->next || !current->next->value)
			{
				print_error("minishell", NULL, "syntax error near '>>'");
				return (0);
			}
			fd = open(current->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				perror(current->next->value);
				return (0);
			}
			if (cmd->output_fd != STDOUT_FILENO)
				close(cmd->output_fd);
			cmd->output_fd = fd;
			current = current->next;
		}
		else if (current->type == TOKEN_HEREDOC)
		{
			if (!current->next || !current->next->value)
			{
				print_error("minishell", NULL, "syntax error near '<<'");
				return (0);
			}
			fd = handle_heredoc(current->value);
			if (fd == -1)
				return (0);
			if (cmd->input_fd != STDIN_FILENO)
				close(cmd->input_fd);
			cmd->input_fd = fd;
			current = current->next;
		}
		current = current->next;
	}
	return (1);
}
