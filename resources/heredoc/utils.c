/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:22:09 by manandre          #+#    #+#             */
/*   Updated: 2025/01/22 10:42:59 by fjilaias         ###   ########.fr       */
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
		start ++;

	end = start;
	while (*end && *end > 32)
		end ++;
	return (ft_substr(start, 0, end - start));
}

// Function to remove heredoc from the command
char	*remove_heredoc(char *cmd)
{
	char	*heredoc_pos;
	char	*clean_cmd;

	heredoc_pos = mini_strstr(cmd, "<<");
	if (!heredoc_pos)
		return (ft_strdup(cmd));

	clean_cmd = malloc(sizeof(char) * (heredoc_pos - cmd + 1));
	if (!clean_cmd)
		return (NULL);
	ft_strlcpy(clean_cmd, cmd, heredoc_pos - cmd + 1);

	return (clean_cmd);
}

// Função principal para lidar com heredoc no meio do pipe
void	handle_heredoc(char *delimiter, int fd_destino[2])
{
	int	pipe_heredoc[2];
	pid_t	pid;
	char    buffer[1024];
	ssize_t	bytes_read;
	(void) fd_destino;

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
		dup2(pipe_heredoc[0], STDIN_FILENO);
		close(pipe_heredoc[0]);
		// Espera o processo do heredoc terminar
		waitpid(pid, NULL, 0);
	}
}
void handle_heredoc_left(char *delimiter, int pipe_fd[2])
{
    pid_t pid;
    char *heredoc_content = NULL;
    size_t heredoc_len = 0;
    char buffer[1024];
    ssize_t bytes_read;

    pid = fork();
    if (pid == -1)
    {
        perror("heredoc fork failed");
        return;
    }

    if (pid == 0)
    {
        // No filho: Fecha o lado de leitura do pipe
        close(pipe_fd[0]);

        while (1)
        {
            ft_putstr_fd("> ", 1);
            bytes_read = read(STDIN_FILENO, buffer, 1023);
            if (bytes_read <= 0)
                break;

            buffer[bytes_read] = '\0';

            // Remove o '\n' do final para comparar com o delimitador
            if (buffer[bytes_read - 1] == '\n')
                buffer[bytes_read - 1] = '\0';

            if (ft_strcmp(buffer, delimiter) == 0) // Se for o delimitador, encerra
                break;

            // Adiciona o conteúdo lido ao conteúdo acumulado
            heredoc_content = realloc(heredoc_content, heredoc_len + bytes_read);
            if (!heredoc_content)
            {
                perror("heredoc realloc failed");
                exit(EXIT_FAILURE);
            }
            ft_memcpy(heredoc_content + heredoc_len, buffer, bytes_read - 1); // Sem o '\0'
            heredoc_content[heredoc_len + bytes_read - 1] = '\n';             // Adiciona o '\n'
            heredoc_len += bytes_read; // Atualiza o tamanho total
        }

        // Escreve todo o conteúdo acumulado no pipe de uma vez
        if (heredoc_content)
        {
            if (write(pipe_fd[1], heredoc_content, heredoc_len) == -1)
                perror("heredoc write failed");
            free(heredoc_content);
        }

        close(pipe_fd[1]); // Fecha o lado de escrita do pipe
        exit(EXIT_SUCCESS);
    }
    else
    {
        // No pai: Fecha o lado de escrita do pipe
        close(pipe_fd[1]);

        // Configura o pipe para ser o novo stdin
        if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
        {
            perror("dup2 failed");
            close(pipe_fd[0]);
            return;
        }
        close(pipe_fd[0]); // Fecha o lado de leitura original após duplicar

        // Aguarda o término do processo filho
        waitpid(pid, NULL, 0);
    }
}
