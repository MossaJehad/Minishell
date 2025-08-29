/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:58:46 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/29 16:35:37 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	extract_key_value(char *arg, char **key, char **value)
{
	size_t	keylen;
	char	*temp_key;
	char	*temp_value;

	keylen = ft_strchr(arg, '=') - arg;
	temp_key = ft_strndup(arg, keylen);
	*key = ft_strtrim(temp_key, "\"");
	free(temp_key);
	temp_value = ft_strdup(ft_strchr(arg, '=') + 1);
	*value = ft_strtrim(temp_value, "\"");
	free(temp_value);
}

void	assign_env_variable(char ***envp, char *key, char *value)
{
	char	*temp_value;
	char	*temp_key;

	if (!is_valid_identifier(key))
	{
		set_shell_status(1);
		perror("export: not a valid identifier");
	}
	else
	{
		temp_value = ft_strjoin(key, "=");
		temp_key = ft_strjoin(temp_value, value);
		add_or_replace(envp, temp_key);
		free(temp_value);
		free(temp_key);
	}
}

void	export_no_args(char **env)
{
	char	**sorted_env;
	char	*eq;
	int		j;

	sorted_env = ft_strdup_array(env);
	ft_sort_array(sorted_env);
	j = 0;
	while (sorted_env[j])
	{
		eq = ft_strchr(sorted_env[j], '=');
		if (eq)
			printf("declare -x %.*s=\"%s\"\n", (int)(eq - sorted_env[j]),
				sorted_env[j], eq + 1);
		else
			printf("declare -x %s\n", sorted_env[j]);
		j++;
	}
	ft_free_arr(sorted_env);
}
