#include "libft.h"

char	*ft_strtok(char *str, const char *delim)
{
	static char	*save;
	char		*token;
	int			i;

	i = 0;
	if (str)
		save = str;
	if (!save || *save == '\0')
		return (NULL);
	while (*save && ft_strchr(delim, *save))
		save++;
	if (*save == '\0')
		return (NULL);
	token = save;
	while (save[i] && !ft_strchr(delim, save[i]))
		i++;
	if (save[i])
	{
		save[i] = '\0';
		save = &save[i + 1];
	}
	else
		save = NULL;
	return (token);
}

char *path_join(const char *dir, const char *file)
{
    char *result = malloc(ft_strlen(dir) + ft_strlen(file) + 2);
    sprintf(result, "%s/%s", dir, file);
    return result;
}

