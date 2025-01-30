/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_forks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:18:01 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/31 00:27:42 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_forks(t_cmd *cmd)
{
	int	i;
	int	status;
	int	exit_status;

	i = -1;
	while (++i < cmd->pid_count)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status))
		{
			exit_status = WEXITSTATUS(status);
			if (exit_status == 1)
				cmd->status_cmd = 1;
			else if (exit_status == 0)
				cmd->status_cmd = 0;
			else if (exit_status == 14)
				cmd->status_cmd = 127;
			else if (exit_status == 2)
				cmd->status_cmd = 127;
			else if (exit_status == 130)
				cmd->status_cmd = exit_status;
			else
				cmd->status_cmd = 126;
		}
	}
}

static char	*aux_exec_heredoc(char *cmd)
{
	char	*heredoc_delim;
	char	*tmp;
	char	*tmp1;

	tmp1 = NULL;
	heredoc_delim = get_heredoc_delimiter(cmd);
	if (heredoc_delim)
	{
		handle_heredoc(heredoc_delim, STDIN_FILENO);
		free(heredoc_delim);
		tmp = ft_strdup(cmd);
		tmp1 = remove_heredoc(tmp);
		free(tmp);
	}
	if (tmp1)
		return (tmp1);
	else
		return (ft_strdup(cmd));
}

char	*aux_exec(char *ccmd, t_cmd *cmd)
{
	char	*tmp;
	char	*tmp1;
	char	*rs;

	tmp = process_cmd(ccmd);
	tmp1 = get_first_word(tmp);
	rs = find_executable(tmp1, &(cmd->g_env_list));
	free(tmp1);
	free(tmp);
	return (rs);
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
		ccmd = aux_exec_heredoc(node->command);
		cmd_not_found_end_exit(ccmd);
		path = aux_exec(ccmd, cmd);
		args = get_args(ccmd);
		if (!path)
			path = ft_strdup("");
		if (execve(path, args, cmd->envl) == -1)
		{
			error_execve(args[0], path, args);
			free(ccmd);
			free_all(cmd);
			exit(errno);
		}
	}
}
