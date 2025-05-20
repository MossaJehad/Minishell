#include "libft.h"

char	*ft_strdup(const char *s)
{
	char *dup;
	size_t i;

	i = ft_strlen(s);
	dup = (char *)malloc(i * sizeof(char) + 1);
	if (!dup)
	{
		free(dup);
		dup = NULL;
		return (0);
	}
	i = 0;
	while (s[i] != '\0')
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}