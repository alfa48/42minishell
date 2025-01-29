/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_left.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 11:35:09 by manandre          #+#    #+#             */
/*   Updated: 2025/01/27 11:00:48 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	mini_close_fd(int fd_0, int fd_1)
{
	close(fd_0);
	close(fd_1);
}

void	configure_stdin(char *heredoc_delim, int *pipefd)
{
	if (heredoc_delim)
	{
		close(pipefd[0]);
		handle_heredoc(heredoc_delim, STDIN_FILENO);
		free(heredoc_delim);
	}
	else
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
	}
}

char	*prepare_command(char *cmd, char *heredoc_delim)
{
	char	*clean_cmd;

	clean_cmd = remove_redirects(cmd);
	if (heredoc_delim)
		clean_cmd = remove_heredoc(clean_cmd);
	return (clean_cmd);
}

void	execute_pipe_left(int pos, t_cmd *cmd)
{
	int			pid;
	char		*heredoc_delim;
	char		*clean_cmd;
	t_redirect	**redirects;

	cmd->pid_count++;
	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
	{
		cmd_not_found_end_exit(cmd->array[pos]);
		printf("execute_pipe_left\n");
		heredoc_delim = get_heredoc_delimiter(cmd->array[pos]);
		close(cmd->pipefd[1]);
		configure_stdin(heredoc_delim, cmd->pipefd);
		redirects = parse_redirects(cmd->array[pos], cmd);
		handle_redirects(redirects);
		clean_cmd = prepare_command(process_cmd(cmd->array[pos]),
				heredoc_delim);
		execute_with_args(clean_cmd, redirects, cmd);
	}
}
