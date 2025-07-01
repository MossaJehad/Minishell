#include "minishell.h"

char *expand_dollar(char *arg) 
{
    char *var_name;
    char *value;
    
    if (ft_strcmp(arg, "$") == 0)
        return ft_strdup("$");
    if (ft_strcmp(arg, "$$") == 0 || ft_strcmp(arg, "$?") == 0)
        return ft_strdup(arg);
    var_name = arg + 1;
    value = getenv(var_name);
    if(value)
		return ft_strdup(value);
	free(arg);
	return ft_strdup("");
}
char *expand_double_quote(char *arg) 
{
    char buffer[BUFFER_SIZE];
    int i; 
    int j;
    
	i = 1;
	j = 0;
    while (arg[i] && arg[i] != '"') {
        if (arg[i] == '$' && arg[i+1] != '"') {
            // Extract variable name
            int k = 0;
            char var_name[256];
            i++; // Skip $
            
            while (ft_isalnum(arg[i]) || arg[i] == '_')
                var_name[k++] = arg[i++];
            var_name[k] = '\0';
            
            // Get value
            char *value = getenv(var_name);
            if (value) {
                strcpy(buffer + j, value);
                j += strlen(value);
            }
        } else {
            buffer[j++] = arg[i++];
        }
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

char	**expand(char **args)
{
	int		i;
	char	*new;

	i = 0;
	new = NULL;
	while (args[i])
	{
		if (args[i][0] == '$')
		{
			new = expand_dollar(args[i]);
			free (args[i]);
			args[i] = new;
		}
		else if (args[i][0] == '\'')
			args[i] = expand_single_quote(args[i]);
		else if (args[i][0] == '"')
			args[i] = expand_double_quote(args[i]);
		i++;
	}
	return (args);
}
