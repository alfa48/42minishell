/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:24:48 by manandre          #+#    #+#             */
/*   Updated: 2025/01/07 10:47:28 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*mini_strcat(char *dest, const char *src)
{
	char    *p_dest;

	p_dest = dest;
	while (*p_dest != '\0')
		p_dest ++;
	while (*src != '\0')
	{
		*p_dest = *src;
		p_dest ++;
		src ++;
	}
	*p_dest = '\0';
	return (dest);
}

static int	get_args_size(char **cmd_args)
{
	int	size;

	size = 0;
	while (cmd_args[size] != NULL)
		size ++;
	return (size);
}

char	**get_args(char *cmd)
{
	char	**cmd_args;
	char	**execve_args;
	int		i;

	i = 0;
	cmd_args = ft_split(cmd, ' ');
	execve_args = malloc((get_args_size(cmd_args) + 1) * sizeof(char *));
	//execve_args[0] =  ft_strjoin("/bin/", cmd_args[0]);
	execve_args[0] =  cmd_args[0];
	while (++i < get_args_size(cmd_args))
		execve_args[i] = cmd_args[i];
	execve_args[i] = NULL;
	return (execve_args);
}

void	execute_command( t_cmd *cmd, t_node *node)
{
		char	**execve_args;

		pid_t pid = fork();
		if (pid == 0)
		{
			execve_args = get_args(node->command);
			fprintf(stderr, "Debug: Comando full %s\n", execve_args[0]);
			execve(execve_args[0], execve_args, cmd->envl);
			perror("execve failed");
			exit(1);
		}
		else if (pid > 0)
		{
			cmd->pid_count ++;
			printf("Um pid adicionado %d\n",cmd->pid_count);
		}
		else
			perror("fork failed");
}

void execute_tree(t_node *root, t_cmd *cmd)//apagar
{
	(void) root;
	(void) cmd;
}



int is_operator(char *str)
{
    if (!str)
        return 0;

    // Lista de operadores válidos
    char *operators[] = {">", "<", ">>", "<<", "|"};
    size_t num_operators = sizeof(operators) / sizeof(operators[0]);
    size_t i = 0;

    // Percorre a lista de operadores usando while
    while (i < num_operators) {
        if (ft_strcmp(str, operators[i]) == 0)
            return 1;
        i++;
    }

    return 0;
}

void execute_commands(int pos, t_cmd *cmd)
{
    if (!cmd->array[pos])
		return;
    execute_commands(pos - 1, cmd);
    // printf("DEBUG: Analisando token '%s'\n", cmd->array[pos]);
    
    // Se é um comando (não um operador)
    if (cmd->array[pos] && !is_operator(cmd->array[pos]))
    {
        // Verifica o próximo token no array    
        if (cmd->array[pos + 1])
        {
            // Se o próximo(right) token é um pipe
            if (ft_strcmp(cmd->array[pos + 1], "|") == 0)
            {
                if (cmd->array[pos - 1])
                    if (ft_strcmp(cmd->array[pos - 1], "|") == 0)// Se o próximo(right) e o anterior  tokens sao  pipes
                    {
                        // printf("DEBUG: LOGICA DO PIPE MIDDLE para '| %s |'\n", cmd->array[pos]);

                        execute_pipe_middle(pos, cmd);
                        return ;
                    }
                // printf("DEBUG: LOGICA DO PIPE RIGHT para '%s |'\n", cmd->array[pos]);
               // Criamos o primeiro pipe
                if (pipe(cmd->pipefd) == -1)
                {
                    perror("pipe failed");
                    return;
                }
                execute_pipe_right(pos, cmd);
                return ;
            }    
            // Se o próximo token é um redirecionamento
            else if (ft_strcmp(cmd->array[pos + 1], ">") == 0 ||
                     ft_strcmp(cmd->array[pos + 1], "<") == 0 ||
                     ft_strcmp(cmd->array[pos + 1], ">>") == 0)
            {
                // printf("DEBUG: LOGICA DOS REDIRECIONAMENTOS para '%s %s %s'\n", 
                    //    cmd->array[pos],
                    //    cmd->array[pos + 1],
                    //    cmd->array[pos + 2]);
                // execute_redirect(pos + 1, cmd);
                //execute_commands(pos + 2, cmd);  // Pula o operador e o arquivo
                return;
            }
            printf("DEBUG: ANTES DO %s É NULL\n", cmd->array[pos]);

        }
        else if (cmd->array[pos - 1])// Se o anterior existir 
        {   
            if (ft_strcmp(cmd->array[pos - 1], "|") == 0)// Se o anterior(left) token é um pipe
            {
                // printf("DEBUG: LOGICA DO PIPE LEFT para '| %s'\n", cmd->array[pos]);
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
        // printf("DEBUG: É UM OPERADOR(%s) NAO FACA NADA\n", cmd->array[pos]);
		return ;
    }
}


void    exec(t_cmd *cmd)
{
	t_node	*tmp_root = cmd->root;
	(void) tmp_root;
    pipe(cmd->pipefd);
	execute_commands(cmd->size, cmd);
	// printf("Debug: root: %d\n", cmd->pid_count);
     close(cmd->pipefd[0]);
	 close(cmd->pipefd[1]);

	wait_forks(cmd);
}