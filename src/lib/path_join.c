#include "libft.h"

char	*ft_free(char **ptr)
{
	if (*ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
	return (NULL);
}

char	*path_join(const char *dir, const char *file)
{
	size_t	len_dir;
	size_t	len_file;
	char	*joined;
	size_t	total_len;
	size_t	i;
	size_t	j;

	len_dir = ft_strlen(dir);
	len_file = ft_strlen(file);
	total_len = len_dir + 1 + len_file + 1;
	joined = (char *)malloc(total_len);
	if (!joined)
		ft_free(&joined);
	i = -1;
	while (++i < len_dir)
		joined[i] = dir[i];
	joined[i++] = '/';
	j = -1;
	while (j < len_file)
		joined[++i] = file[++j];
	joined[i] = '\0';
	return (joined);
}
