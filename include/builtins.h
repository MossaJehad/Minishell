/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:04:33 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/20 03:00:04 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "./minishell.h"


typedef struct s_shell			t_shell;

/* cd */
int		change_directory(char *target_dir, char ***envp);
void	handle_cd_command(char *path, int arg_count, char ***envp);
char	*resolve_cd_target(char *path, char **envp);
char	*determine_cd_target(char *path, char **envp);

/* echo */
int	check_flag(char **flag, int *idx);
int		ft_echo(int nb, char **arg, char **envp);
void	last_check(int flag);

/* env */
int		ft_env(char **envp);

/* exit */
void	ft_exit(t_shell *shell, char **cmd);
void	simple_exit(t_shell *shell, int code);
void	exit_code(t_shell *shell, char **args);
void	exit_code_in_pipes(t_shell *shell, char **arg);

/* exit_utils */
int		test_max_min(char *arg);
int		is_exit_correct(t_shell *shell, char *arg, int i);
void	exit_err(t_shell *shell);
void	simple_exit2(t_shell *shell, int code);

/* export */
int		ft_export(t_shell *shell);

/* export_utils */
void	sort_array(char **array);
void	ft_env_export(char **envp);
char	**ft_realloc_array(char **old_envp, int new_size);
char	*get_var_name(t_shell *shell, char *str);
int		is_valid_var(char *str);

/* pwd */
void	update_pwd_oldpwd(char ***envp, const char *new_pwd,
		const char *old_pwd);
void	handle_pwd_command(char ***envp);

/* unset */
void	remove_env_var(char ***envp, const char *name);
void	handle_unset_command(char ***envp, char **args, int arg_count);

#endif
