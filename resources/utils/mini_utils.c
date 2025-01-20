/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:24:48 by manandre          #+#    #+#             */
/*   Updated: 2025/01/14 14:48:08 by fjilaias         ###   ########.fr       */
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


void execute_tree(t_node *root, t_cmd *cmd)//apagar
{
	(void) root;
	(void) cmd;
}



int is_operator(char *str)
{
    if (!str)
        return 0;

    char *operators[] = {">", "<", ">>", "<<", "|"};
    size_t num_operators = sizeof(operators) / sizeof(operators[0]);
    size_t i = 0;

    while (i < num_operators) {
        if (ft_strcmp(str, operators[i]) == 0)
            return 1;
        i++;
    }

    return 0;
}

int is_redirect(char *str)
{
    if (!str)
        return 0;

    char *operators[] = {">", "<", ">>", "<<"};
    size_t num_operators = sizeof(operators) / sizeof(operators[0]);
    size_t i = 0;
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

// Função auxiliar para verificar precedência de redirecionamentos
int get_redirect_priority(char *type) {
    if (ft_strcmp(type, "<") == 0) return 1;  // Input tem prioridade mais alta
    if (ft_strcmp(type, ">") == 0) return 2;  // Output normal
    if (ft_strcmp(type, ">>") == 0) return 2; // Append tem mesma prioridade que output
    return 0;
}

// Função para ordenar redirecionamentos por precedência
void sort_redirects(t_redirect **redirects) {
    int i, j;
    t_redirect *temp;

    if (!redirects) return;

    for (i = 0; redirects[i] && redirects[i + 1]; i++) {
        for (j = 0; redirects[j] && redirects[j + 1]; j++) {
            if (get_redirect_priority(redirects[j]->type) >
                get_redirect_priority(redirects[j + 1]->type)) {
                temp = redirects[j];
                redirects[j] = redirects[j + 1];
                redirects[j + 1] = temp;
            }
        }
    }
}

void execute_single_command(char *cmd_str, t_cmd *cmd)
{
    int pid;
    char *path = NULL;
    char **args = NULL;
    t_redirect **redirects = NULL;
    char *clean_cmd = NULL;
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);

    if (!cmd_str || !cmd)
    {
        if (saved_stdin != -1) close(saved_stdin);
        if (saved_stdout != -1) close(saved_stdout);
        return;
    }

    // Parse os redirecionamentos do comando
    redirects = parse_redirects(cmd_str);
    if (!redirects)
    {
        close(saved_stdin);
        close(saved_stdout);
        return;
    }

    // Obtém o comando limpo antes do fork
    clean_cmd = remove_redirects(cmd_str);
    if (!clean_cmd)
    {
        free_redirects(redirects);
        close(saved_stdin);
        close(saved_stdout);
        return;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        free(clean_cmd);
        free_redirects(redirects);
        close(saved_stdin);
        close(saved_stdout);
        return;
    }

    if (pid == 0) // Processo filho
    {
        int opened_fds[MAX_REDIRECTS * 2];
        int fd_count = 0;

        // Aplica os redirecionamentos na ordem correta
        for (int i = 0; redirects[i]; i++)
        {
            int new_fd = -1;

            if (ft_strcmp(redirects[i]->type, "<") == 0)
                new_fd = open(redirects[i]->file, O_RDONLY);
            else if (ft_strcmp(redirects[i]->type, ">") == 0)
                new_fd = open(redirects[i]->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            else if (ft_strcmp(redirects[i]->type, ">>") == 0)
                new_fd = open(redirects[i]->file, O_WRONLY | O_CREAT | O_APPEND, 0644);

            if (new_fd == -1)
            {
                perror("open failed");
                goto cleanup_child;
            }

            opened_fds[fd_count++] = new_fd;

            if (ft_strcmp(redirects[i]->type, "<") == 0)
            {
                if (dup2(new_fd, STDIN_FILENO) == -1)
                {
                    perror("dup2 failed");
                    goto cleanup_child;
                }
            }
            else
            {
                if (dup2(new_fd, STDOUT_FILENO) == -1)
                {
                    perror("dup2 failed");
                    goto cleanup_child;
                }
            }
        }

        // Prepara argumentos e executa
        path = find_executable(get_first_word(ft_strdup(clean_cmd)), &(cmd->g_env_list));
        if (!path)
        {
            cmd_not_found(get_first_word(ft_strdup(clean_cmd)));
            goto cleanup_child;
        }

        args = get_args(clean_cmd);
        if (!args)
            goto cleanup_child;

        execve(path, args, cmd->envl);
        perror("execve failed");

cleanup_child:
        // Limpa recursos no processo filho
        for (int i = 0; i < fd_count; i++)
            if (opened_fds[i] != -1)
                close(opened_fds[i]);
        free(clean_cmd);
        free(path);
        if (args)
            free_array(args);
        free_redirects(redirects);
        exit(EXIT_FAILURE);
    }
    else // Processo pai
    {
        int status;
        
        // Espera o filho terminar
        waitpid(pid, &status, 0);
        
        // Restaura os file descriptors
        if (saved_stdin != -1)
        {
            dup2(saved_stdin, STDIN_FILENO);
            close(saved_stdin);
        }
        if (saved_stdout != -1)
        {
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdout);
        }
        
        // Limpa recursos
        free(clean_cmd);
        free_redirects(redirects);
        
        if (WIFEXITED(status))
        {
            cmd->status_cmd = WEXITSTATUS(status);
        }
    }
}



void    exec(t_cmd *cmd)
{
	t_node	*tmp_root = cmd->root;
	(void) tmp_root;

    pipe(cmd->pipefd);
    if (ft_strchr(cmd->line1, '|'))//Forma de saber se tem pipe
    {
	    printf("Debug: ROOT PIPE: %s\n", cmd->line);
	    execute_commands(cmd->size, cmd);// se tem pipe
    }
    else if (has_redirect(cmd->line1))
    {
	    printf("Debug: ROOT REDIRECT apagar: %s\n", cmd->line);
        execute_single_command(cmd->line1, cmd);

        //exec_command_redirect(1, cmd);//se nao tem pipe mas tem red
    }
    else{
        char        *heredoc_delim;
        char        *path;
        char        **args;
        char *clean_cmd;
        (void)args;
        (void)clean_cmd;
        (void)path;
        (void)heredoc_delim;

          // Primeiro, verifica se tem heredoc
        printf("Debug: get_heredoc_delimiter \n");//, heredoc_delim);

        heredoc_delim = get_heredoc_delimiter(cmd->line1);
    }
	printf("Debug: root:\n");
    close(cmd->pipefd[0]);
	close(cmd->pipefd[1]);

	wait_forks(cmd);
}
