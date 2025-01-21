/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_forks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:18:01 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/21 11:48:48 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_forks(t_cmd *cmd)
{
	int	i;
	int	status;
	int	exit_status;

	i = 0;
	while (i < cmd->pid_count)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status))
		{
			exit_status = WEXITSTATUS(status);
			if (exit_status == 1)
				cmd->status_cmd = 127;
			else if (exit_status == 0)
				cmd->status_cmd = 0;
			else
				cmd->status_cmd = 126;
		}
		i++;
	}
}

char	*fork_exec_heredoc(char *cmd)
{
	char	*heredoc_delim;

	heredoc_delim = get_heredoc_delimiter(cmd);
	if (heredoc_delim)
	{
		handle_heredoc(heredoc_delim, STDIN_FILENO);
		free(heredoc_delim);
		cmd = remove_heredoc(cmd);
	}
	return (cmd);
}

void	fork_exec_cmd(t_cmd *cmd, t_node *node)
{
	int		pid;
	char	*path;
	char	**args;
	char	*ccmd;

	pid = fork();
	cmd->pid_count++;
	if (pid == 0)
	{
		ccmd = node->command;
		ccmd = fork_exec_heredoc(ccmd);
		if (is_entirely_within_quotes(ccmd))
			path = find_executable((process_cmd(ccmd)), &(cmd->g_env_list));
		else
			path = find_executable(get_first_word(process_cmd(ccmd)),
					&(cmd->g_env_list));
		args = get_args(ccmd);
		ccmd = args[0];
		if (execve(path, args, cmd->envl) == -1)
		{
			cmd_not_found(ccmd);
			free(path);
			exit(1);
		}
	}
}
