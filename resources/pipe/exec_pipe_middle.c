/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_middle.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 11:30:30 by manandre          #+#    #+#             */
/*   Updated: 2025/01/23 16:48:36 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipe_creation(t_cmd *cmd)
{
	cmd->pid_count++;
	ft_memcpy(cmd->prev_pipe, cmd->pipefd, 2 * sizeof(int));
	if (pipe(cmd->pipefd) == -1)
	{
		perror("pipe failed");
		exit(EXIT_FAILURE);
	}
}

void	handle_heredoc_or_input(int prev_pipe_read, char *heredoc_delim)
{
	if (heredoc_delim)
	{
		close(prev_pipe_read);
		handle_heredoc(heredoc_delim, STDIN_FILENO);
		free(heredoc_delim);
	}
	else
		dup2(prev_pipe_read, STDIN_FILENO);
}

void	configure_redirects(t_redirect **redirects)
{
	int	i;

	i = 0;
	while (redirects && redirects[i])
	{
		if (ft_strcmp(redirects[i]->type, ">") == 0)
		{
			redirects[i]->fd = open(redirects[i]->file,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (redirects[i]->fd != -1)
				dup2(redirects[i]->fd, STDOUT_FILENO);
		}
		else if (ft_strcmp(redirects[i]->type, ">>") == 0)
		{
			redirects[i]->fd = open(redirects[i]->file,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (redirects[i]->fd != -1)
				dup2(redirects[i]->fd, STDOUT_FILENO);
		}
		i++;
	}
}

void	execute_clean_command(char *cmd, t_redirect **redirects,
		t_cmd *cmd_info)
{
	char	*clean_cmd;
	char	*path;
	char	**args;

	clean_cmd = remove_redirects(cmd);
	path = find_executable(get_first_word(ft_strdup(clean_cmd)),
			&(cmd_info->g_env_list));
	args = get_args(clean_cmd);
	free(clean_cmd);
	if (redirects)
		free_redirects(redirects);
	if (execve(path, args, cmd_info->envl) == -1)
		error_execve(args[0], path, args);
}

void	execute_pipe_middle_(int pos, t_cmd *cmd)
{
	int			pid;
	char		*heredoc_delim;
	t_redirect	**redirects;

	heredoc_delim = NULL;
	redirects = NULL;
	handle_pipe_creation(cmd);
	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
	{
		heredoc_delim = get_heredoc_delimiter(cmd->array[pos]);
		close(cmd->prev_pipe[1]);
		close(cmd->pipefd[0]);
		handle_heredoc_or_input(cmd->prev_pipe[0], heredoc_delim);
		dup2(cmd->pipefd[1], STDOUT_FILENO);
		close(cmd->pipefd[1]);
		redirects = parse_redirects(cmd->array[pos], cmd);
		configure_redirects(redirects);
		execute_clean_command(cmd->array[pos], redirects, cmd);
	}
	else
		mini_close_fd(cmd->prev_pipe[0], cmd->prev_pipe[1]);
}
