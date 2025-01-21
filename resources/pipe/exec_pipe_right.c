/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_right.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 11:32:54 by manandre          #+#    #+#             */
/*   Updated: 2025/01/21 12:00:43 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_pipe_right(int pos, t_cmd *cmd)
{
	int pid;
	char *path;
	char **args;
	t_redirect **redirects = NULL;
	char *heredoc_delim = NULL;

	cmd->pid_count++;
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		return ;
	}

	if (pid == 0)
	{
		heredoc_delim = get_heredoc_delimiter(cmd->array[pos]);
		if (heredoc_delim)
		{
			handle_heredoc_left(heredoc_delim, cmd->pipefd);
			free(heredoc_delim);
		}
		else
		{
			redirects = parse_redirects(cmd->array[pos]);
			setup_io(redirects, NULL, cmd->pipefd, false);
		}
		close(cmd->pipefd[0]);
		close(cmd->pipefd[1]);

		char *clean_cmd = remove_redirects(cmd->array[pos]);
		if (heredoc_delim)
			clean_cmd = remove_heredoc(clean_cmd);

		path = find_executable(get_first_word(ft_strdup(clean_cmd)),
				&(cmd->g_env_list));
		args = get_args(clean_cmd);

		free(clean_cmd);
		if (redirects)
			free_redirects(redirects);

		if (execve(path, args, cmd->envl) == -1)
		{
			cmd_not_found(get_first_word(ft_strdup(cmd->array[pos])));
			free(path);
			free_array(args);
			exit(EXIT_FAILURE);
		}
	}
}