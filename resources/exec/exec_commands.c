/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 11:21:36 by manandre          #+#    #+#             */
/*   Updated: 2025/01/21 11:22:21 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execute_commands(int pos, t_cmd *cmd)
{
    if (!cmd->array[pos])
		return ;
    execute_commands(pos - 1, cmd);

      if (pos > 0 && cmd->array[pos - 1])
          if (is_redirect(cmd->array[pos - 1]))
              return ;
    // printf("DEBUG: Analisando token '%s'\n", cmd->array[pos]);

    // Se é um comando (não um operador)
    if (cmd->array[pos] && !is_operator(cmd->array[pos]))
    {
        // Verifica o próximo token no array
        if (pos < cmd->size && cmd->array[pos + 1])
        {
            // Se o próximo(right) token é um pipe
            if (ft_strcmp(cmd->array[pos + 1], "|") == 0)
            {
                if (pos > 0 && cmd->array[pos - 1])
                    if (ft_strcmp(cmd->array[pos - 1], "|") == 0)// Se o próximo(right) e o anterior  tokens sao  pipes
                    {
                        printf("DEBUG: LOGICA DO PIPE MIDDLE para '| %s |'\n", cmd->array[pos]);

                        execute_pipe_middle_(pos, cmd);
                        return ;
                    }
                printf("DEBUG: LOGICA DO PIPE RIGHT para '%s |'\n", cmd->array[pos]);
               // Criamos o primeiro pipe
                if (pipe(cmd->pipefd) == -1)
                {
                    perror("pipe failed");
                    return;
                }
                execute_pipe_right(pos, cmd);
                return ;
            }
            printf("DEBUG: ANTES DO %s É NULL\n", cmd->array[pos]);
        }
        else if (pos > 0 && cmd->array[pos - 1])// Se o anterior existir
        {
            if (ft_strcmp(cmd->array[pos - 1], "|") == 0)// Se o anterior(left) token é um pipe
            {
                printf("DEBUG: LOGICA DO PIPE LEFT para '| %s'\n", cmd->array[pos]);
                execute_pipe_left(pos, cmd);
                 // Fecha pipes após último comando
                close(cmd->pipefd[0]);
                close(cmd->pipefd[1]);
                return ;
            }
            return ;
        }
        //fork_exec_cmd_(cmd, cmd->array[pos]);
        return ;
    }
    else if (cmd->array[pos])
    {
        printf("DEBUG: É UM OPERADOR(%s) NAO FACA NADA\n", cmd->array[pos]);
		return ;
    }
}