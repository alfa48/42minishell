/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_signal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 18:02:38 by manandre          #+#    #+#             */
/*   Updated: 2025/01/27 16:46:49 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_sig_status_cmd(t_cmd *cmd)
{
	printf("SIGNAL: %d\n", g_signal_status);
	if (g_signal_status)
		cmd->status_cmd = g_signal_status;
	g_signal_status = 0;
}

/*void	handle_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}*/

void	sigint_handler(int signum)
{
	(void)signum;
	if (signum == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
    		rl_replace_line("", 0);
    		rl_redisplay();
		if (!g_signal_status)
			g_signal_status = 130;
	}
}
