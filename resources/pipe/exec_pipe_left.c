/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_left.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 11:35:09 by manandre          #+#    #+#             */
/*   Updated: 2025/01/21 11:35:28 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    execute_pipe_left(int pos, t_cmd *cmd)
{
    int         pid;
    char        *path;
    char        **args;
    t_redirect  **redirects;
    char        *heredoc_delim;

    cmd->pid_count++;
    pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        return;
    }

    if (pid == 0)
    {
        // Primeiro verifica se tem heredoc
        heredoc_delim = get_heredoc_delimiter(cmd->array[pos]);

        // Fecha pipe de escrita que não será usado
        close(cmd->pipefd[1]);

        if (heredoc_delim)
        {
            // Se tem heredoc, fecha a leitura do pipe
            // pois vamos usar apenas o heredoc
            close(cmd->pipefd[0]);
            
            // Configura o heredoc como entrada padrão
            handle_heredoc(heredoc_delim, STDIN_FILENO);
            free(heredoc_delim);
        }
        else
        {
            // Se não tem heredoc, usa a entrada do pipe
            dup2(cmd->pipefd[0], STDIN_FILENO);
            close(cmd->pipefd[0]);
        }

        // Parse outros redirecionamentos
        redirects = parse_redirects(cmd->array[pos]);
        
        // Aplica outros redirecionamentos se houver
        if (redirects)
        {
            int i = 0;
            while (redirects[i])
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
}
