/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:55:27 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/22 15:09:09 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_heredoc_delimiter(char *cmd)
{
	char	*start;
	char	*end;

	if (is_within_quotes(cmd, "<<"))
		return (NULL);
	start = mini_strstr(cmd, "<<");
	if (!start)
		return (NULL);
	start += 2;
	while (*start && *start <= 32)
		start++;
	end = start;
	while (*end && *end > 32)
		end++;
	return (ft_substr(start, 0, end - start));
}

static int	create_pipe_and_fork(int pipe_heredoc[2], pid_t *pid)
{
	if (pipe(pipe_heredoc) == -1)
	{
		perror("heredoc pipe failed");
		return (-1);
	}
	*pid = fork();
	if (*pid == -1)
	{
		perror("heredoc fork failed");
		close(pipe_heredoc[0]);
		close(pipe_heredoc[1]);
		return (-1);
	}
	return (0);
}

static void	process_heredoc_child(int pipe_heredoc[2], char *delimiter)
{
	char	buffer[1024];
	ssize_t	bytes_read;

	close(pipe_heredoc[0]);
	while (1)
	{
		ft_putstr_fd("> ", 1);
		bytes_read = read(STDIN_FILENO, buffer, 1023);
		if (bytes_read <= 0)
			break ;
		buffer[bytes_read] = '\0';
		if (buffer[bytes_read - 1] == '\n')
			buffer[bytes_read - 1] = '\0';
		if (ft_strcmp(buffer, delimiter) == 0)
			break ;
		buffer[bytes_read - 1] = '\n';
		write(pipe_heredoc[1], buffer, bytes_read);
	}
	close(pipe_heredoc[1]);
	exit(EXIT_SUCCESS);
}

static void	process_heredoc_parent(int pipe_heredoc[2], pid_t pid)
{
	close(pipe_heredoc[1]);
	dup2(pipe_heredoc[0], STDIN_FILENO);
	close(pipe_heredoc[0]);
	waitpid(pid, NULL, 0);
}

void	handle_heredoc(char *delimiter, int fd_destino[2])
{
	int		pipe_heredoc[2];
	pid_t	pid;

	(void)fd_destino;
	if (create_pipe_and_fork(pipe_heredoc, &pid) == -1)
		return ;
	if (pid == 0)
		process_heredoc_child(pipe_heredoc, delimiter);
	else
		process_heredoc_parent(pipe_heredoc, pid);
}
