/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_right.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 11:32:54 by manandre          #+#    #+#             */
/*   Updated: 2025/01/30 16:14:04 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*aux_heredoc_right(char *cmd, int pipefd[2])
{
	char	*heredoc_delim;

	heredoc_delim = get_heredoc_delimiter(cmd);
	if (heredoc_delim)
	{
		handle_heredoc_left(heredoc_delim, pipefd);
		free(heredoc_delim);
		cmd = remove_heredoc(cmd);
	}
	return (cmd);
}

void	execute_pipe_right(int pos, t_cmd *cmd)
{
	int			pid;
	char		*var_str;
	char		*clean_cmd;
	t_redirect	**redirects;

	cmd->pid_count++;
	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
	{
		cmd_not_found_end_exit(cmd->array[pos]);
		var_str = aux_heredoc_right(cmd->array[pos], cmd->pipefd);
		redirects = parse_redirects(var_str, cmd);
		setup_io(redirects, NULL, cmd->pipefd, false);
		close(cmd->pipefd[0]);
		close(cmd->pipefd[1]);
		clean_cmd = remove_redirects(var_str);
		execute_with_args(clean_cmd, redirects, cmd);
	}
}

void	execute_with_args(char *clean_cmd, t_redirect **redirects, t_cmd *cmd)
{
	char	*path;
	char	**args;

	path = aux_exec(clean_cmd, cmd);
	args = get_args(clean_cmd);
	if (!path)
		path = ft_strdup("");
	if (redirects)
		free_redirects(redirects);
	if (execve(path, args, cmd->envl) == -1)
	{
		error_execve(args[0], path, args);
		free(clean_cmd);
		free_all(cmd);
		exit(errno);
	}
}

void	error_execve(char *ccmd, char *path, char **args)
{
	cmd_not_found(ccmd);
	free(path);
	free_array(args);
}
