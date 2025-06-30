#include "minishell.h"

void	handle_command(char *input,
                       char **args,
                       int arg_count,
                       t_token *token,
                       char ***envp)
{
	char	cwd[1024];
	char	*cmd;

	cmd = args[0];
	if (ft_strcmp(cmd, "exit") == 0 && arg_count > 1
		&& ft_strcmp(args[1], "0") == 0)
		exit(0);
	else if (ft_strcmp(cmd, "echo") == 0)
		handle_echo_command(token);
	else if (ft_strcmp(cmd, "env") == 0)
		handle_env_command(*envp);
	else if (ft_strcmp(cmd, "export") == 0)
		handle_export_command(envp, args, arg_count);
	else if (ft_strcmp(cmd, "type") == 0)
	{
		if (arg_count >= 2)
			for (int i = 1; i < arg_count; i++)
				handle_type_command(args[i]);
	}
	else if (ft_strcmp(cmd, "pwd") == 0)
		printf("%s\n", getcwd(cwd, sizeof(cwd)));
    else if (ft_strcmp(cmd, "unset") == 0)
        handle_unset_command(envp, args, arg_count);
    else if (ft_strcmp(cmd, "cd") == 0)
		handle_cd_command(input + 2, arg_count);
	else
		printf("%s: command not found\n", input);
}

char *get_input(void)
{
    char *line;
    char *new_part;
    char *combined;
    int empty_line;

    line = NULL;
    new_part = NULL;
    combined = NULL;
    empty_line = 0;
    while (1) {
        prompt();
        line = readline(empty_line ? "> " : "");
        empty_line = 0;
        
        if (!line) {
            printf("exit\n");
            return NULL;
        }
        if (*line == '\0') {
            free(line);
            empty_line = 1;
            continue;
        }
        if (!has_unclosed_quotes(line)) {
            if (*line)
                add_history(line);
            return line;
        }
        while (1) {
            new_part = readline("> ");
            if (!new_part) {
                free(line);
                return NULL;
            }
            combined = ft_strjoin(line, "\n");
            free(line);
            line = ft_strjoin(combined, new_part);
            free(combined);
            free(new_part);
            if (!has_unclosed_quotes(line))
                break;
        }
        if (*line)
            add_history(line);
        return line;
    }
}

void	prompt(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s$ ", cwd);
	else
		printf("$ ");
}
