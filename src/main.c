#include "minishell.h"

// #include "libft.h"
////// LATEST
// Copies characters from src to buffer, interpreting escape sequences if inside a quoted string.
int		whileloopstring(int i, int j, int len, char *buffer, const char *src, int bufsize, int string)
{
	while (i < len && j < bufsize - 1)
	{
		if (src[i] == '\\' && i + 1 < len)
		{
			if (string && src[i + 1] == 'n')
				buffer[j++] = '\n';
			else if (src[i] == 't')
				buffer[j++] = '\t';
			else if (src[i] == '\\')
				buffer[j++] = '\\';
			else if (src[i] == '"')
				buffer[j++] = '"';
			else if (src[i] == '\'')
				buffer[j++] = '\'';
			else
				buffer[j++] = src[i];
			i++;
		}
		else
			buffer[j++] = src[i++];
	}
	buffer[j] = '\0';
	return j;
}
// Removes surrounding quotes and processes escape sequences in a string.
char	*unescape_string(const char *src)
{
	static char	buffer[1024];
	char		*processed;
	int			i;
	int			j;
	size_t		len;
	int			string;

	processed = buffer;
	i = 0;
	j = 0;
	string = 0;
	len = ft_strlen(src);
	if (len >= 2 && ((src[0] == '\'' && src[len - 1] == '\'') || (src[0] == '"' && src[len - 1] == '"')))
	{
		src++;
		len -= 2;
		string = 1;
	}
	j = whileloopstring(i, j, len, buffer, src, sizeof(buffer), string);
	return (processed);
}

// Splits the input line into arguments while respecting quotes and escape characters.
char	**parse_arguments(const char *input, int *arg_count, int *quote_error)
{
	static char	*argv[64];
	char		buffer[1024];
	int			in_single_quote;
	int			in_double_quote;
	int			k;
	int			j;
	int			i;
	char		c;

	i = 0;
	j = 0;
	k = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (input[i])
	{
		c = input[i];
		if (c == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			i++;
			continue ;
		}
		if (c == '"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			i++;
			continue ;
		}
		if (!in_single_quote && !in_double_quote && (c == ' ' || c == '\t'))
		{
			if (j > 0)
			{
				buffer[j] = '\0';
				argv[k++] = ft_strdup(buffer);
				j = 0;
			}
			i++;
			continue ;
		}
		if (c == '\\' && input[i + 1])
		{
			if(input[i] == '\\')
				i++;
			buffer[j++] = input[i++];
			continue ;
		}
		buffer[j++] = c;
		i++;
	}
	if (j > 0)
	{
		buffer[j] = '\0';
		argv[k++] = ft_strdup(buffer);
	}
	argv[k] = NULL;
	*arg_count = k;
	*quote_error = in_single_quote || in_double_quote;
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
// Runs the main shell loop that reads user input, parses it, and executes commands.
void	shell_loop(char **envp, int arg_count)
{
	char	*input;
	char	**args;
	int		quote_error;
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
		tokenize(args, &token);
		handle_command(input, args, arg_count, envp, token);
		ft_free(args);
		free_tokens(token);
	}
}


int	main(int argc, char **argv, char **envp)
{	
	(void)argc;
	(void)argv;
	init_shell();
	shell_loop(envp, argc);
	rl_clear_history();
	return (0);
}
