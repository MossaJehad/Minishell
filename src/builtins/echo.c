/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:06:42 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/08/20 02:43:57 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	last_check(int flag)
{
	if (flag == 0)
		write(1, "\n", 1);
}

int	check_flag(char **flag, int *idx)
{
	int		i;
	int		j;
	int	ret;

	i = 1;
	if (flag[i] == NULL)
		return (0);
	while (flag[i] && flag[i][0] == '-')
	{
		j = 1;
		while (flag[i][j] == 'n')
			j++;
		if (flag[i][j] == '\0')
		{
			(*idx)++;
			i++;
		}
		else
			break ;
	}
	ret = *idx > 0;
	return (ret);
}

int	ft_echo(int nb, char **arg, char **envp)
{
	int		i;
	int		j;
	int	flag;

	i = 0;
	flag = check_flag(arg, &i);
	while (++i < nb)
	{
		j = 0;
		while (arg[i][j] && arg[i][j + 1])
		{
			if (arg[i][j] == '$' && ft_isupper(arg[i][++j]))
			{
				if (arg[i + 1] && ft_getenv(arg[++i], envp) == NULL)
					ft_putstr_fd("\n", 1);
			}
			else
				break ;
		}
		ft_putstr_fd(arg[i], 1);
		if (i != nb - 1)
			ft_putchar_fd(' ', 1);
	}
	last_check(flag);
	return (0);
}
