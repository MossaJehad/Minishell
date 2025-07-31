/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhasoneh <mhasoneh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:41:17 by mhasoneh          #+#    #+#             */
/*   Updated: 2025/07/31 15:41:53 by mhasoneh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_sigquit(int sig)
{
	(void)sig;
}

void	restore_signals(void)
{
	setup_signal_handlers();
}

int	get_exit_status(void)
{
	return (g_exit_status);
}

void	set_exit_status(int status)
{
	g_exit_status = status;
}
