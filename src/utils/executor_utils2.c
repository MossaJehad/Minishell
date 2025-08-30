#include "../../include/minishell.h"

int is_dir (char *cmd, int flag)
{
    struct stat	sb;

	if (!stat(cmd, &sb) && S_ISDIR(sb.st_mode))
	{
		if (ft_strcmp(cmd, ".") == 0 || ft_strcmp(cmd, "..") == 0)
            return (127);
		else
		{
            if (flag)
            {
			    write(2, cmd, ft_strlen(cmd));
                write(2, ": Is a directory\n", 18);
            }
            return (126);
		}
	}
    return (0);
}
