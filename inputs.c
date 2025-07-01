#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int	setup_redirection(t_token *tok)
{
	int fd;

	if (ft_strcmp(tok->type, "redirect input") == 0)
		fd = open(tok->value, O_RDONLY);
	else if (ft_strcmp(tok->type, "redirect output") == 0)
		fd = open(tok->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else /* append output */
		fd = open(tok->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror(tok->value), -1);
	if (ft_strcmp(tok->type, "redirect input") == 0)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

void	handle_command(char *input,
                     char **args,
                     int arg_count,
                     t_token *token,
                     char ***envp)
{
    int saved_in  = dup(STDIN_FILENO);
    int saved_out = dup(STDOUT_FILENO);
    char *exec_args[MAX_ARGS];
    int  ei = 0;
    t_token *cur = token;
    pid_t pid;

    // 1) collect redirections
    while (cur)
    {
        if (ft_strcmp(cur->type, "redirect input") == 0
         || ft_strcmp(cur->type, "redirect output") == 0
         || ft_strcmp(cur->type, "append output") == 0)
        {
            // mark but do not apply yet
        }
        cur = cur->next;
    }
    // 2) build exec_args (skip redir tokens + filenames)
    for (int i = 0; i < arg_count; ++i)
    {
        if (ft_strcmp(args[i], "<") == 0
         || ft_strcmp(args[i], ">") == 0
         || ft_strcmp(args[i], ">>") == 0
         || ft_strcmp(args[i], "<<") == 0)
        {
            i++; continue;
        }
        exec_args[ei++] = args[i];
    }
    exec_args[ei] = NULL;

    pid = fork();
    if (pid == 0)
    {
        // child: apply all redir tokens
        cur = token;
        while (cur)
        {
            if (ft_strcmp(cur->type, "redirect input") == 0
             || ft_strcmp(cur->type, "redirect output") == 0
             || ft_strcmp(cur->type, "append output") == 0)
            {
                if (setup_redirection(cur) == -1)
                    exit(1);
            }
            cur = cur->next;
        }
        // child: dispatch
        if (exec_args[0] && ft_strcmp(exec_args[0], "echo") == 0)
            handle_echo_command(token);
        else if (exec_args[0] && ft_strcmp(exec_args[0], "env") == 0)
            handle_env_command(*envp);
        else if (exec_args[0] && ft_strcmp(exec_args[0], "export") == 0)
            handle_export_command(envp, exec_args, ei);
        else if (exec_args[0] && ft_strcmp(exec_args[0], "type") == 0)
        {
            for (int j = 1; j < ei; ++j)
                handle_type_command(exec_args[j]);
        }
        else if (exec_args[0] && ft_strcmp(exec_args[0], "pwd") == 0)
        {
            char cwd[1024];
            printf("%s\n", getcwd(cwd, sizeof(cwd)));
        }
        else if (exec_args[0] && ft_strcmp(exec_args[0], "cd") == 0)
            handle_cd_command(input + 2, ei);
        else if (exec_args[0] && ft_strcmp(exec_args[0], "exit") == 0
              && ei > 1 && ft_strcmp(exec_args[1], "0") == 0)
            exit(0);
        else if (exec_args[0] && ft_strcmp(exec_args[0], "unset") == 0)
            handle_unset_command(envp, exec_args, ei);
        else
            execvp(exec_args[0], exec_args), perror(exec_args[0]);
        exit(0);
    }
    else if (pid > 0)
    {
        waitpid(pid, NULL, 0);
        // restore parent stdio
        dup2(saved_in, STDIN_FILENO);
        dup2(saved_out, STDOUT_FILENO);
    }
    else
        perror("fork");
}

char *get_input(void)
{
    char *line;
    char *new_part;
    char *combined;

    line       = NULL;
    new_part   = NULL;
    combined   = NULL;
    while (1)
    {
        // build and pass cwd-based prompt into readline()
        {
            char  cwd[1024];
            char *prompt;
            if (getcwd(cwd, sizeof(cwd)))
            {
                char *tmp = ft_strdup(cwd);
                prompt    = ft_strjoin(tmp, "$ ");
                free(tmp);
            }
            else
                prompt = ft_strdup("$ ");
            line = readline(prompt);
            free(prompt);
        }
        if (!line)
        {
            printf("exit\n");
            return NULL;
        }
        if (*line == '\0')
        {
            free(line);
            continue;
        }
        if (!has_unclosed_quotes(line))
        {
            add_history(line);
            return line;
        }
        // multi-line continuation
        while (has_unclosed_quotes(line))
        {
            new_part = readline("> ");
            if (!new_part)
            {
                free(line);
                return NULL;
            }
            combined = ft_strjoin(line, "\n");
            free(line);
            line = ft_strjoin(combined, new_part);
            free(combined);
            free(new_part);
        }
        add_history(line);
        return line;
    }
}


