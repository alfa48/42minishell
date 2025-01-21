/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_middle.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 11:30:30 by manandre          #+#    #+#             */
/*   Updated: 2025/01/21 11:31:22 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execute_pipe_middle_(int pos, t_cmd *cmd)
{
    int         pid;
    char        *path;
    char        **args;
    t_redirect  **redirects;
    char        *heredoc_delim;
    int         i;

    cmd->pid_count++;
    ft_memcpy(cmd->prev_pipe, cmd->pipefd, 2 * sizeof(int));

    if (pipe(cmd->pipefd) == -1)
    {
        perror("pipe failed");
        return;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        return;
    }

    if (pid == 0)
    {
        // Primeiro, verifica se tem heredoc
        heredoc_delim = get_heredoc_delimiter(cmd->array[pos]);
        
        // Fecha os pipes não utilizados primeiro
        close(cmd->prev_pipe[1]);  // Fecha escrita do pipe anterior
        close(cmd->pipefd[0]);     // Fecha leitura do próximo pipe
        
        if (heredoc_delim)
        {
            // Se tem heredoc, fecha a leitura do pipe anterior
            // pois vamos usar apenas o heredoc
            close(cmd->prev_pipe[0]);
            
            // Configura o heredoc
            handle_heredoc(heredoc_delim, STDIN_FILENO);
            free(heredoc_delim);
        }
        else
        {
            // Se não tem heredoc, usa o pipe anterior
            dup2(cmd->prev_pipe[0], STDIN_FILENO);
        }

        // Configura a saída para o próximo comando
        dup2(cmd->pipefd[1], STDOUT_FILENO);
        close(cmd->pipefd[1]);

        // Parse redirects (para outros redirecionamentos além do heredoc)
        redirects = parse_redirects(cmd->array[pos]);
        
        // Aplica outros redirecionamentos, se houver
        if (redirects)
        {
            i = 0;
            while (redirects[i])
            {
                // Só aplica redirecionamentos que não sejam heredoc
                if (ft_strcmp(redirects[i]->type, "<<") != 0)
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
                }
                i++;
            }
        }

        // Limpa o comando e executa
        char *clean_cmd = remove_redirects(cmd->array[pos]);
        if (heredoc_delim)
            clean_cmd = remove_heredoc(clean_cmd);

        path = find_executable(get_first_word(ft_strdup(clean_cmd)), 
            &(cmd->g_env_list));
        args = get_args(clean_cmd);

        // Cleanup antes do exec
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
    else
    {
        // Processo pai
        close(cmd->prev_pipe[0]);
        close(cmd->prev_pipe[1]);
    }
}