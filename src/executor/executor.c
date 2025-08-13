/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 17:11:46 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/13 05:23:27 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_child_builtin(char *cmd_argv[MAX_ARGS], int cmd_argc,
		char **envp)
{
	t_token	*cmd_token;
	char	cwd[PATH_MAX];
	int		k;

	if (!ft_strcmp(cmd_argv[0], "echo"))
	{
		cmd_token = NULL;
		create_token(&cmd_token, cmd_argv[0], "command");
		k = 1;
		while (k < cmd_argc)
		{
			create_token(&cmd_token, cmd_argv[k], "word");
			k++;
		}
		handle_echo_command(cmd_token);
		free_tokens(cmd_token);
	}
	else if (!ft_strcmp(cmd_argv[0], "pwd"))
	{
		if (getcwd(cwd, sizeof(cwd)))
			printf("%s\n", cwd);
		else
			perror("pwd");
	}
	else if (!ft_strcmp(cmd_argv[0], "env"))
	{
		handle_env_command(envp);
	}
}



int	prepare_child_command(t_token *seg, char *cmd_argv[MAX_ARGS])
{
	int	cmd_argc;

	cmd_argc = 0;
	while (seg && ft_strcmp(seg->value, "pipe") != 0)
	{
		if (ft_strncmp(seg->value, "redirect", 8) == 0 || ft_strcmp(seg->value,
				"append output") == 0)
		{
			if (setup_redirection(seg) == -1)
				return (-1);
			seg = seg->next;
			continue ;
		}
		else if (ft_strcmp(seg->value, "here-document") == 0)
		{
			seg = seg->next;
			continue ;
		}
		cmd_argv[cmd_argc++] = seg->value;
		seg = seg->next;
	}
	cmd_argv[cmd_argc] = NULL;
	return (cmd_argc);
}

char	*find_executable(const char *cmd, t_shell *shell)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	char	*temp;
	int		i;

	if (!cmd)
		return (NULL);

	/* If command contains '/', it's already a path */
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (safe_strdup(cmd));
		return (NULL);
	}

	/* Get PATH environment variable */
	path_env = env_get(shell, "PATH");
	if (!path_env)
		return (NULL);

	/* Split PATH and search for executable */
	paths = safe_split(path_env, ':');
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);

		if (access(full_path, X_OK) == 0)
		{
			free_string_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}

	free_string_array(paths);
	return (NULL);
}

void	execute_single_command(t_shell *shell, t_cmd *cmd)
{
	char	**env_array;
	char	*executable;

	if (!cmd || !cmd->args || !cmd->args[0])
	{
		shell->exit_status = EXIT_SUCCESS;
		return;
	}

	/* Handle built-in commands */
	if (is_builtin(cmd->args[0]))
	{
		shell->exit_status = execute_builtin(shell, cmd);
		return;
	}

	/* External command */
	executable = find_executable(cmd->args[0], shell);
	if (!executable)
	{
		print_error(cmd->args[0], NULL, "command not found");
		shell->exit_status = EXIT_COMMAND_NOT_FOUND;
		return;
	}

	/* Set up file descriptors */
	if (cmd->input_fd != STDIN_FILENO)
		dup2(cmd->input_fd, STDIN_FILENO);
	if (cmd->output_fd != STDOUT_FILENO)
		dup2(cmd->output_fd, STDOUT_FILENO);

	/* Execute command */
	env_array = env_to_array(shell);
	setup_child_signals();
	
	if (execve(executable, cmd->args, env_array) == -1)
	{
		perror("execve");
		free(executable);
		free_string_array(env_array);
		exit(EXIT_CANNOT_EXECUTE);
	}
}

void	setup_pipe_child(int *pipe_fds, int cmd_index, int num_cmds)
{
	/* Setup input from previous pipe */
	if (cmd_index > 0)
	{
		dup2(pipe_fds[(cmd_index - 1) * 2], STDIN_FILENO);
	}

	/* Setup output to next pipe */
	if (cmd_index < num_cmds - 1)
	{
		dup2(pipe_fds[cmd_index * 2 + 1], STDOUT_FILENO);
	}

	/* Close all pipe file descriptors */
	close_pipe_fds(pipe_fds, num_cmds - 1);
}

int	fork_and_execute(t_shell *shell, t_cmd *cmd, int *pipe_fds, 
							int cmd_index, int num_cmds)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}

	if (pid == 0)
	{
		/* Child process */
		setup_pipe_child(pipe_fds, cmd_index, num_cmds);
		
		/* Setup redirections */
		if (cmd->input_fd != STDIN_FILENO)
			dup2(cmd->input_fd, STDIN_FILENO);
		if (cmd->output_fd != STDOUT_FILENO)
			dup2(cmd->output_fd, STDOUT_FILENO);
		
		execute_single_command(shell, cmd);
		exit(shell->exit_status);
	}

	return (pid);
}

void	execute_pipeline(t_shell *shell, t_cmd *commands)
{
	int		num_cmds;
	int		*pipe_fds;
	t_cmd	*current;
	int		i;

	num_cmds = count_commands(commands);
	if (num_cmds == 1)
	{
		execute_single_command(shell, commands);
		return;
	}

	/* Create pipes */
	pipe_fds = (int *)safe_malloc(sizeof(int) * 2 * (num_cmds - 1));
	i = 0;
	while (i < num_cmds - 1)
	{
		if (pipe(pipe_fds + i * 2) == -1)
		{
			perror("pipe");
			free(pipe_fds);
			shell->exit_status = EXIT_FAILURE;
			return;
		}
		i++;
	}

	/* Fork processes and setup pipes */
	shell->pids = (pid_t *)safe_malloc(sizeof(pid_t) * num_cmds);
	shell->num_processes = num_cmds;
	
	current = commands;
	i = 0;
	while (current && i < num_cmds)
	{
		shell->pids[i] = fork_and_execute(shell, current, pipe_fds, i, num_cmds);
		if (shell->pids[i] == -1)
		{
			shell->exit_status = EXIT_FAILURE;
			break;
		}
		current = current->next;
		i++;
	}

	/* Close all pipe file descriptors in parent */
	close_pipe_fds(pipe_fds, num_cmds - 1);
	free(pipe_fds);

	/* Wait for all processes */
	wait_for_all_processes(shell);
}

void	wait_for_all_processes(t_shell *shell)
{
	int		status;
	int		i;
	pid_t	pid;

	if (!shell->pids || shell->num_processes == 0)
		return;

	i = 0;
	while (i < shell->num_processes)
	{
		pid = waitpid(shell->pids[i], &status, 0);
		if (pid == shell->pids[shell->num_processes - 1])
		{
			/* Set exit status based on last process */
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGINT)
					shell->exit_status = EXIT_CTRL_C;
				else if (WTERMSIG(status) == SIGQUIT)
					shell->exit_status = EXIT_CTRL_BACKSLASH;
				else
					shell->exit_status = 128 + WTERMSIG(status);
			}
		}
		i++;
	}

	free(shell->pids);
	shell->pids = NULL;
	shell->num_processes = 0;
}

void	execute_commands(t_shell *shell, t_cmd *commands)
{
	if (!commands)
		return;

	/* Check if single builtin that should run in parent process */
	if (!commands->next && commands->args && commands->args[0] && 
		is_builtin(commands->args[0]) && should_run_in_parent(commands->args[0]))
	{
		/* Setup redirections for builtin */
		if (commands->input_fd != STDIN_FILENO)
			dup2(commands->input_fd, STDIN_FILENO);
		if (commands->output_fd != STDOUT_FILENO)
			dup2(commands->output_fd, STDOUT_FILENO);
		
		shell->exit_status = execute_builtin(shell, commands);
		
		/* Restore original file descriptors */
		if (commands->input_fd != STDIN_FILENO)
		{
			dup2(STDIN_FILENO, STDIN_FILENO);
			close(commands->input_fd);
		}
		if (commands->output_fd != STDOUT_FILENO)
		{
			dup2(STDOUT_FILENO, STDOUT_FILENO);
			close(commands->output_fd);
		}
		return;
	}

	/* Execute pipeline or single external command */
	execute_pipeline(shell, commands);
}


void	execute_child_process(t_token *cmd_starts[256], int i,
		int heredoc_fds[256], int pipefd[256][2], int num_cmds, char **envp)
{
	t_token	*seg;
	char	*cmd_argv[MAX_ARGS];
	int		cmd_argc;
	char	*full_path;

	seg = cmd_starts[i];
	setup_child_signals();
	setup_child_pipes(pipefd, i, num_cmds);
	setup_child_heredoc(heredoc_fds, i);
	cmd_argc = prepare_child_command(seg, cmd_argv);
	if (cmd_argc == -1)
		exit(1);
	if (cmd_argc == 0 || cmd_argv[0] == NULL)
		exit(0);
	if (is_builtin(cmd_argv[0]))
	{
		execute_child_builtin(cmd_argv, cmd_argc, envp);
		exit(0);
	}
	if (cmd_argv[0][0] == '/' || cmd_argv[0][0] == '.')
		full_path = ft_strdup(cmd_argv[0]);
	else
		full_path = find_executable(cmd_argv[0], (void *)envp);
	if (!full_path)
	{
		perror(cmd_argv[0]);
		exit(127);
	}
	execve(full_path, cmd_argv, envp);
	perror(full_path);
	free(full_path);
	exit(127);
}

int	fork_processes(t_token *cmd_starts[256], int num_cmds, int heredoc_fds[256],
		int pipefd[256][2], pid_t pids[256], char **envp)
{
	int	i;

	i = 0;
	while (i < num_cmds)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			perror("fork");
			set_shell_status(1);
			return (-1);
		}
		if (pids[i] == 0)
		{
			execute_child_process(cmd_starts, i, heredoc_fds, pipefd, num_cmds,
				envp);
		}
		i++;
	}
	return (0);
}
