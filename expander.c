#include "minishell.h"

static char *lookup_env(const char *name, char **envp)
{
    size_t len = ft_strlen(name);
    int    i = 0;
    while (envp && envp[i])
    {
        if (!ft_strncmp(envp[i], name, len) && envp[i][len] == '=')
            return (&envp[i][len + 1]);
        i++;
    }
    return NULL;
}

char *expand_dollar(char *arg, char **envp) 
{
    char *var = arg + 1;
    char *val;

    if (!ft_strcmp(arg, "$"))
        return ft_strdup("$");
    if (!ft_strcmp(arg, "$$") || !ft_strcmp(arg, "$?"))
        return ft_strdup(arg);
    val = lookup_env(var, envp);
    free(arg);
    return ft_strdup(val ? val : "");
}

char *expand_double_quote(char *arg, char **envp) 
{
    char buffer[BUFFER_SIZE];
    int  i = 1, j = 0;
    char *val;

    while (arg[i] && arg[i] != '"')
    {
        if (arg[i] == '$' && arg[i+1] != '"' && arg[i+1])
        {
            int  k = 0;
            char var[256];
            i++;
            while (ft_isalnum(arg[i]) || arg[i] == '_')
                var[k++] = arg[i++];
            var[k] = '\0';
            val = lookup_env(var, envp);
            if (val)
            {
                ft_strlcpy(buffer + j, val, ft_strlen(val) + 1);
                j += ft_strlen(val);
            }
        }
        else
            buffer[j++] = arg[i++];
    }
    buffer[j] = '\0';
    free(arg);
    return ft_strdup(buffer);
}

char	*expand_single_quote(char *arg)
{
	char	*str;
	int		i;
	int		j;
	int		len;

	len = ft_strlen(arg);
	i = 1;
	j = 0;
	if (len < 2)
		return (ft_strdup(""));
	str = malloc(len - 1);
	if (!str)
	{
		ft_free(&str);
		return (NULL);
	}
	while (i < len - 1)
	{
		str[j++] = arg[i++];
	}
	str[j] = '\0';
	free(arg);
	return (str);
}

char **expand(char **args, char **envp)
{
    int  i = 0;
    char *new;

    while (args[i])
    {
        if (args[i][0] == '$')
        {
            new = expand_dollar(args[i], envp);
            args[i] = new;
        }
        else if (args[i][0] == '\'')
            args[i] = expand_single_quote(args[i]);
        else if (args[i][0] == '"')
            args[i] = expand_double_quote(args[i], envp);
        i++;
    }
    return args;
}
