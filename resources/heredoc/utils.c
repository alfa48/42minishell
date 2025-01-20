/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:22:09 by manandre          #+#    #+#             */
/*   Updated: 2025/01/16 13:22:10 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Função auxiliar para obter o delimitador do heredoc
char	*get_heredoc_delimiter(char *cmd)
{
	char	*start;
	char	*end;

	start = mini_strstr(cmd, "<<");
	if (!start)
		return (NULL);
    
	start += 2;  // Pula o "<<"
	while (*start && *start <= 32)
		start ++;

	end = start;
	while (*end && *end > 32)
		end ++;
	return (ft_substr(start, 0, end - start));
}

// Função para remover o heredoc do comando
char	*remove_heredoc(char *cmd)
{
	char	*heredoc_pos;
	char	*clean_cmd;

	heredoc_pos = mini_strstr(cmd, "<<");
	if (!heredoc_pos)
		return (ft_strdup(cmd));

	// Aloca espaço para o comando sem o heredoc
	clean_cmd = malloc(sizeof(char) * (heredoc_pos - cmd + 1));
	if (!clean_cmd)
		return (NULL);
	// Copia apenas a parte antes do heredoc
	ft_strlcpy(clean_cmd, cmd, heredoc_pos - cmd + 1);

	return (clean_cmd);
}

// Função principal para lidar com heredoc no meio do pipe
void	handle_heredoc(char *delimiter, int fd_destino)
{
	int	pipe_heredoc[2];
	pid_t	pid;
	char    buffer[1024];
	ssize_t	bytes_read;

	if (pipe(pipe_heredoc) == -1)
	{
		perror("heredoc pipe failed");
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("heredoc fork failed");
		close(pipe_heredoc[0]);
		close(pipe_heredoc[1]);
		return ;
	}
	if (pid == 0)
	{
		// Processo filho para ler o heredoc
		close(pipe_heredoc[0]);
		while (1)
		{
			// Mostra o prompt do heredoc
			ft_putstr_fd("> ", 1);
			// Lê uma linha do stdin
			bytes_read = read(STDIN_FILENO, buffer, 1023);
			if (bytes_read <= 0)
				break ;
			buffer[bytes_read] = '\0';
			// Remove o \n do final se existir
			if (buffer[bytes_read - 1] == '\n')
				buffer[bytes_read - 1] = '\0';            
			// Verifica se é o delimitador
			if (ft_strcmp(buffer, delimiter) == 0)
				break ;
			// Adiciona o \n de volta para a escrita
			buffer[bytes_read - 1] = '\n';
			write(pipe_heredoc[1], buffer, bytes_read);
		}
		close(pipe_heredoc[1]);
		exit(EXIT_SUCCESS);
	}
	else
	{
		// Processo pai
		close(pipe_heredoc[1]);        
		// Redireciona a leitura do heredoc para o fd_destino
		dup2(pipe_heredoc[0], fd_destino);
		close(pipe_heredoc[0]);        
		// Espera o processo do heredoc terminar
		waitpid(pid, NULL, 0);
	}
}
