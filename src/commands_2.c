#include "minishell.h"

int     valid_identifier(char *str)
{
	int     i;

    i = 0;
	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void    remove_env_var(t_data *data, char *str)
{
	int     i;
    int     j;
	int     len;
	char    **new_env;
    
    i = 0;
    j = 0;
    len = ft_strlen(str);
    new_env = malloc(sizeof(char *) * (env_len(data->env)));
	if (!new_env)
		return;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], str, len) == 0 && data->env[i][len] == '=')
			i++;
		else
			new_env[j++] = ft_strdup(data->env[i++]);
	}
	new_env[j] = NULL;
	ft_free_split(data->env);
	data->env = new_env;
}

void    handle_unset_command(char **args, int arg_count, t_data *data)
{
    int     i;

    i = 1;
    if(!args[i])
    {
        data->last_status = 0;
        return;
    }
    while(args[i])
    {
        if(!valid_identifier(args[i]))
        {
            ft_printf("unset: %s: not a valid identifier\n", args[i]);
            data->last_status = 1;
        }
        else
        {
            remove_env_var(data, args[i]);
            data->last_status = 0;
        }
        i++;
    }
}

void    handle_env_command(char **args, t_data *data)
{
    int     i;

    i = 0;
    if(args[1])
    {
        ft_printf("env: too many arguments\n");
        data->last_status = 2;
        return;
    }
    while(data->env[i])
    {
        ft_printf("%s\n", data->env[[i]]);
        i++;
    }
    data->last_status = 0;
}
