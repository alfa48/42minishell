/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:24:48 by manandre          #+#    #+#             */
/*   Updated: 2024/11/20 12:24:49 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"


static int get_args_size(char **cmd_args)
{
    int size = 0;
    while (cmd_args[size] != NULL) {
        size++;
    }
    return size;
}

char    **get_args(char *cmd)
{
        char **cmd_args = ft_split(cmd, ' ');

        char **execve_args = malloc((get_args_size(cmd_args) + 1) * sizeof(char *));
        execve_args[0] =  ft_strjoin("/bin/", cmd_args[0]);
        int i = 0;
        while (++i < get_args_size(cmd_args))
            execve_args[i] = cmd_args[i];
        execve_args[i] = NULL;
        i = -1;
       /* while (cmd_args[++i] != NULL) {
            printf("cmd_args[%d]: %s\n", i, execve_args[i]);  // Exibe o argumento
    }*/
    return (execve_args);
}

int    is_prev_pipe(char **array, int index)
{
    if (index > 1)
        if (!ft_strcmp(array[index - 1], "|"))
            return (1);
    return (0);
}

int    is_next_pipe(char **array, int size, int index)
{
    //printf("proximo é |\n");
    if (index < size)
        if (!ft_strcmp(array[index], "|"))
            return (1);
    return (0);
}
/*
void    exec1(char **array, int index)
{
    printf("Ver o proximo cmd e executar este cmd...\n");


    if (ft_strcmp(array[index], "|"))
        exec_pipe();
    else if (ft_strcmp(array[index], ">"))
        exc_redirect(array[index]);  
    else if (ft_strcmp(array[index], "<"))
        exc_redirect(array[index]);
    else if (ft_strcmp(array[index], ">>"))
        exc_redirect(array[index]);  
    else if (ft_strcmp(array[index], "<<"))
        exc_redirect(array[index]);  
}
*/


void    exec2()
{
   // printf("executar este cmd...\n");
}

void	traverse_tree(t_node *root, char **array, int size)
{
	if (!root || !array)
		return ;

    traverse_tree(root->left, array, size);

    if (root->command)
    {
        /*IF (O NÓ ANTERIOR É UM PIPE)
           
        if (is_prev_pipe(array, root->index))
        {
  
            {PEGA O CONTEUDO DEIXADO PELO COMANDO ANTERIOR NO PIPE E USAR COMO ARGUMENTO}*/
                   /*/ IF (O NÓ POSTERIOR É UM PIPE)
            */
        if (is_next_pipe(array, size, root->index))
        {
            //{EXECUTAR O cmd1 E GUARDAR O RESULTADO NO PIPE criar o filho para aexec cm2 com o resultado do cmd1}
            int fd[2];
            pid_t pid_1;
            pid_t pid_2;
                char *matr[] = {"PATH=/bin", NULL};
                
            
            pipe(fd);
            pid_1 = fork();//exec cm1
            //printf("PID: %d\n", pid_1);
            if (pid_1 == 0)
            {   
                //printf("P1 ESTA SENDO EXECUTADO: %s\n", root->command);
                char **execve_args = get_args(root->command);

              //  char *args1[] = {ft_strjoin("/bin/", cmd_args[0]) , &cmd_args[1], NULL};
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]); // Fecha o lado de leitura
                close(fd[1]); // Fecha o lado de ESCRITA
                if (mini_strstr(root->command, "echo") != NULL)
                     mini_echo(root->command);
                else
                    execve(execve_args[0], execve_args, matr);
                exit(1);
            }
            //criar processo para exec cmd2
            waitpid(pid_1, NULL, 0);

            pid_2 = fork();
            if (pid_2 == 0)
            {   
                //printf("P2 ESTA SENDO EXECUTADO: \n");
                char **execve_args1 = get_args(array[root->index + 1]);
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]); // Fecha o lado de leitura
                close(fd[1]); // Fecha o lado de ESCRITA
                //printf("CMD depois do pipe %s", array[root->index + 1]);
                if (mini_strstr(root->command, "echo") != NULL)//tem logica
                     mini_echo(root->command);
                else
                    execve(execve_args1[0], execve_args1, matr);
                exit(1);
            }
            close(fd[0]); // Fecha o lado de leitura do pipe
            close(fd[1]); // Fecha o lado de escrita do pipe
            
            waitpid(pid_2, NULL, 0);
        }

    }

}

void    exec(t_cmd *cmd)
{
    printf("%s\n", cmd->root->operator);
    traverse_tree(cmd->root, cmd->array, cmd->size);
}
