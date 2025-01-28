/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_signal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 18:02:38 by manandre          #+#    #+#             */
/*   Updated: 2025/01/28 10:52:52 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_sig_status_cmd(t_cmd *cmd)
{
	if (g_signal_status)
		cmd->status_cmd = g_signal_status;
	g_signal_status = 0;
}

void	handle_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	sigint_handler(int signum)
{
	int pipefd[2];

	(void)signum;
	if (signum == SIGINT)
	{
		pipe(pipefd);
		write(pipefd[1], "\n", 1);
    	dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);



		printf("\n");
		rl_on_new_line();
    	rl_replace_line("", 0);
		if (!g_signal_status)
			g_signal_status = 130;
	}
}
