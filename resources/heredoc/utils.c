/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:22:09 by manandre          #+#    #+#             */
/*   Updated: 2025/01/24 10:22:50 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Parte 1: Fork e controle do processo
static void	fork_and_prepare_pipe(pid_t *pid, int pipe_fd[2])
{
	(void)pipe_fd;
	*pid = fork();
	if (*pid == -1)
	{
		perror("heredoc fork failed");
		exit(EXIT_FAILURE);
	}
}

// Parte 2: Escrita e limpeza do heredoc
static void	write_and_cleanup(int write_fd, char *content, size_t length)
{
	if (content)
	{
		if (write(write_fd, content, length) == -1)
			perror("heredoc write failed");
		free(content);
	}
	close(write_fd);
	exit(EXIT_SUCCESS);
}

// Parte 3: Leitura do heredoc
static void	read_heredoc_content(char *delimiter, int write_fd)
{
	char	*heredoc_content;
	size_t	heredoc_len;
	char	buffer[1024];
	ssize_t	bytes_read;

	heredoc_content = NULL;
	heredoc_len = 0;
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
		heredoc_content = ft_realloc(heredoc_content, heredoc_len + bytes_read);
		ft_memcpy(heredoc_content + heredoc_len, buffer, bytes_read - 1);
		heredoc_content[heredoc_len + bytes_read - 1] = '\n';
		heredoc_len += bytes_read;
	}
	write_and_cleanup(write_fd, heredoc_content, heredoc_len);
}

// Parte 4: Configuração do pipe no processo pai
static void	setup_parent_pipe(pid_t pid, int pipe_fd[2])
{
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		perror("dup2 failed");
		close(pipe_fd[0]);
		exit(EXIT_FAILURE);
	}
	close(pipe_fd[0]);
	waitpid(pid, NULL, 0);
}

// Parte 5: Função principal
void	handle_heredoc_left(char *delimiter, int pipe_fd[2])
{
	pid_t	pid;

	fork_and_prepare_pipe(&pid, pipe_fd);
	if (pid == 0)
	{
		close(pipe_fd[0]);
		read_heredoc_content(delimiter, pipe_fd[1]);
	}
	else
	{
		setup_parent_pipe(pid, pipe_fd);
	}
}
