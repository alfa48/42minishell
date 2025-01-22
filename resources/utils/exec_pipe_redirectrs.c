#include "minishell.h"
void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
    
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i ++;
	}
	free(array);
}

// Verifica se um caractere marca o início de um redirecionamento
static int	is_redirect_char(char c)
{
	return (c == '<' || c == '>');
}





// Função que remove redirecionamentos do comando
char	*remove_redirects(const char *cmd)
{
	bool	in_quotes;
	bool	in_redirect;
	char	*result;
	char	*write_ptr;
	char	quote_char;
	char	*start;
	const char	*read_ptr;

	if (!cmd)
		return (NULL);
	result = malloc(ft_strlen(cmd) + 1);
	write_ptr = result;
	read_ptr = cmd;
	in_quotes = false;
	quote_char = 0;
	in_redirect = false;
	while (*read_ptr)
	{
		// Gerencia aspas
		if (*read_ptr == '"' || *read_ptr == '\'')
		{
			if (!in_quotes)
			{
				in_quotes = true;
				quote_char = *read_ptr;
			}
			else if (*read_ptr == quote_char)
				in_quotes = false;
		}
		// Se estamos dentro de aspas, copia tudo literalmente
		if (in_quotes)
		{
			*write_ptr ++ = *read_ptr ++;
			continue ;
		}
		// Detecta início de redirecionamento
		if (is_redirect_char(*read_ptr) && !in_redirect)
		{
			in_redirect = true;
			// Pula o redirecionador e o arquivo
			while (*read_ptr && (*read_ptr == '<' || *read_ptr == '>'))
				read_ptr ++;
			// Pula espaços
			while (*read_ptr && isspace(*read_ptr))
				read_ptr ++;
			// Pula o nome do arquivo
			while (*read_ptr && !isspace(*read_ptr))
				read_ptr ++;
			// Remove espaços extras
			while (*read_ptr && isspace(*read_ptr))
				read_ptr ++;
			in_redirect = false;
			continue ;
		}
		// Copia caractere normal
		*write_ptr ++ = *read_ptr ++;
	}
	*write_ptr = '\0';
	// Remove espaços extras no final
	write_ptr --;
	while (write_ptr >= result && isspace(*write_ptr))
		write_ptr --;
	*(write_ptr + 1) = '\0';
	// Remove espaços extras no início
	start = result;
	while (*start && isspace(*start))
		start ++;
	if (start != result)
		ft_memmove(result, start, strlen(start) + 1);
	return (result);
}

// Função para liberar a estrutura de redirecionamentos
void	free_redirects(t_redirect **redirects)
{
	int	i;

	if (!redirects)
		return ;
	i = 0;
	while (redirects[i])
	{
		free(redirects[i]->type);
		free(redirects[i]->file);
		free(redirects[i]);
		i ++;
	}
	free(redirects);
}

// Função para extrair redirecionamentos de um comando
t_redirect  **parse_redirects(char *cmd_str)
{
    int     i;
    int     redirect_count;
    t_redirect  **redirects;
    char    **tokens;

    redirects = malloc(sizeof(t_redirect*) * MAX_REDIRECTS);
    tokens = ft_split(cmd_str, ' ');
    i = 0;
    redirect_count = 0;
    while (tokens[i] && redirect_count < MAX_REDIRECTS)
    {
        if (is_redirect(tokens[i]))
        {
            redirects[redirect_count] = malloc(sizeof(t_redirect));
            redirects[redirect_count]->type = ft_strdup(tokens[i]);
            redirects[redirect_count]->file = ft_strdup(tokens[i + 1]);
            redirects[redirect_count]->fd = -1;
            redirect_count++;
            i += 2;
        }
        else
            i ++;
    }
    redirects[redirect_count] = NULL;
    free_array(tokens);
    return (redirects);
}

// Função para aplicar redirecionamentos
void apply_redirects(t_redirect **redirects, int prev_pipe[2], int next_pipe[2])
{
    for (int i = 0; redirects[i]; i++) {
        if (ft_strcmp(redirects[i]->type, "<") == 0) {
            // Input redirection
            redirects[i]->fd = open(redirects[i]->file, O_RDONLY);
            if (redirects[i]->fd == -1) {
                perror("open failed");
                return;
            }
            dup2(redirects[i]->fd, STDIN_FILENO);
        } else if (ft_strcmp(redirects[i]->type, ">") == 0) {
            // Output redirection
            redirects[i]->fd = open(redirects[i]->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (redirects[i]->fd == -1) {
                perror("open failed");
                return;
            }
            dup2(redirects[i]->fd, STDOUT_FILENO);
        } else if (ft_strcmp(redirects[i]->type, ">>") == 0) {
            // Append output redirection
            redirects[i]->fd = open(redirects[i]->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (redirects[i]->fd == -1) {
                perror("open failed");
                return;
            }
            dup2(redirects[i]->fd, STDOUT_FILENO);
        }
    }
    
    // Se houver pipe anterior, conecta à entrada (a menos que já tenha redirecionamento de entrada)
    if (prev_pipe && !has_input_redirect(redirects)) {
        dup2(prev_pipe[0], STDIN_FILENO);
    }
    
    // Se houver próximo pipe, conecta à saída (a menos que já tenha redirecionamento de saída)
    if (next_pipe && !has_output_redirect(redirects)) {
        dup2(next_pipe[1], STDOUT_FILENO);
    }
}

void execute_pipe_middle(int pos, t_cmd *cmd)
{
    int pid;
    char *path;
    char **args;
	 t_redirect **redirects;

    cmd->pid_count++;
	// Primeiro, salvamos o pipe atual como anterior
	ft_memcpy(cmd->prev_pipe, cmd->pipefd, 2 * sizeof(int));
	// Depois criamos o novo pipe
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

    if (pid == 0) // Processo filho
    {
		 // Parse e aplica redirecionamentos
        redirects = parse_redirects(cmd->array[pos]);
        apply_redirects(redirects, cmd->prev_pipe, cmd->pipefd);

        // Redireciona entrada padrão para o pipe de leitura
        //dup2(cmd->prev_pipe[0], STDIN_FILENO);

        // Redireciona saída padrão para o pipe de escrita
        //dup2(cmd->pipefd[1], STDOUT_FILENO);
        // Fecha os descritores do pipe (já redirecionados)
        // Fecha todos os descritores de pipe
        close(cmd->prev_pipe[0]);
        close(cmd->prev_pipe[1]);
        close(cmd->pipefd[0]);
        close(cmd->pipefd[1]);


		// Obtém comando limpo (sem redirecionamentos)
        char *clean_cmd = remove_redirects(cmd->array[pos]);
        // Obtém o caminho do executável e os argumentos
        path = find_executable(get_first_word(ft_strdup(clean_cmd)), &(cmd->g_env_list));
        args = get_args(clean_cmd);

		        // Limpa recursos
        free_redirects(redirects);
        free(clean_cmd);

        // Executa o comando
        if (execve(path, args, cmd->envl) == -1)
        {
            cmd_not_found(get_first_word(ft_strdup(cmd->array[pos])));
            free(path);
            exit(EXIT_FAILURE);
        }
    }
	else{

         close(cmd->prev_pipe[0]);
         close(cmd->prev_pipe[1]);
	}

}



// Helper function to apply redirections consistently across all pipe functions
void apply_redirections(t_redirect **redirects, int read_fd, int write_fd)
{
    t_redirect **redirects_head = redirects;
    
    // Handle file redirections first
    while (redirects && *redirects)
    {
        if (ft_strcmp((*redirects)->type, ">") == 0)
        {
            (*redirects)->fd = open((*redirects)->file, 
                O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if ((*redirects)->fd != -1)
            {
                dup2((*redirects)->fd, STDOUT_FILENO);
                close((*redirects)->fd);
            }
        }
        else if (ft_strcmp((*redirects)->type, ">>") == 0)
        {
            (*redirects)->fd = open((*redirects)->file, 
                O_WRONLY | O_CREAT | O_APPEND, 0644);
            if ((*redirects)->fd != -1)
            {
                dup2((*redirects)->fd, STDOUT_FILENO);
                close((*redirects)->fd);
            }
        }
        else if (ft_strcmp((*redirects)->type, "<") == 0)
        {
            (*redirects)->fd = open((*redirects)->file, O_RDONLY);
            if ((*redirects)->fd != -1)
            {
                dup2((*redirects)->fd, STDIN_FILENO);
                close((*redirects)->fd);
            }
        }

        redirects++;
    }
    
    // Apply pipe redirections if no file redirections override them
    if (!has_input_redirect(redirects_head) && read_fd != -1)
        dup2(read_fd, STDIN_FILENO);
    if (!has_output_redirect(redirects_head) && write_fd != -1)
        dup2(write_fd, STDOUT_FILENO);
        
    // Free resources
    if (redirects_head)
        free_redirects(redirects_head);
}

void	exec_redout(t_node *node, char **env, t_cmd *cmd)
{
	int	fd;
	(void)env;

	pid_t	pid = fork();
	if (pid == 0)
	{
		fd = open(node->right->command, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("open failed for output file");
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		execute_tree(node->left, cmd);
		exit(0);
	}
	waitpid(pid, NULL, 0);
}

void	exec_redin(t_node *node, char **env, t_cmd *cmd)
{
	int	fd;
	(void)env;

	pid_t	pid = fork();
	if (pid == 0)
	{
		fd = open(node->right->command, O_RDONLY);
		if (fd == -1)
		{
			perror("open failed for input file");
			exit(1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
		execute_tree(node->left, cmd);
		exit(0);
	}
	waitpid(pid, NULL, 0);
}

void	exec_redout_append(t_node *node, char **env, t_cmd *cmd)
{
	int	fd;
	(void)env;

	pid_t	pid = fork();
	if (pid == 0)
	{
		// Processo filho: abre o arquivo em modo de anexar
		fd = open(node->right->command, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			perror("open failed for output file");
			exit(1);
		}
		// Redireciona stdout para o descritor de arquivo
		dup2(fd, STDOUT_FILENO);
		close(fd);
		// Executa o comando na subárvore esquerda
		execute_tree(node->left, cmd);
		exit(0);
	}
	// Processo pai: espera o término do processo filho
	waitpid(pid, NULL, 0);
}
/*
void	execute_redirect(t_node *node, char **env,  t_cmd *cmd)
{
	if (node->operator)
	{
		if (ft_strcmp(node->operator, ">") == 0)
			exec_redout(node, env, cmd);
		else if (ft_strcmp(node->operator, "<") == 0)
			exec_redin(node, env, cmd);
		else if (ft_strcmp(node->operator, ">>") == 0)
			exec_redout_append(node, env, cmd);
		else if (ft_strcmp(node->operator, "<<") == 0)
			exec_heredoc(node, env, cmd);
		else
			fprintf(stderr, "Erro: Operador desconhecido '%s'\n", node->operator);
	}
}*/





























void	exec_redout_(int pos, t_cmd *cmd)
{
	int	fd;
	char *path;
	char **args;
	(void)path;
	(void)cmd;
	(void)args;

    cmd->pid_count++;

	pid_t	pid = fork();
	if (pid == 0)
	{
		if (!cmd->array_redirect[pos + 1] || !cmd->array_redirect[pos + 1])
			exit(1);
		fd = open(cmd->array_redirect[pos + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("open failed for output file");
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);// e' mesmo necessario usar o dup2 ou posso usar o dup
		close(fd);
		//fork_exec_cmd_(pos - 1, cmd);

		//path = find_executable(get_first_word(ft_strdup(cmd->array_redirect[pos - 1])), &(cmd->g_env_list));
		//args = get_args(cmd->array_redirect[pos - 1]);
		// ft_putstr_fd("Executa o comando: ", 2);
		// ft_putstr_fd(cmd->array_redirect[pos - 1], 2);
		// ft_putchar_fd('\n', 2);
		// if (execve(path, args, cmd->envl) == -1)
		// {
		// 	free(path);
		// 	exit(EXIT_FAILURE);
		// }
	}
}

void	exec_redin_(int pos, t_cmd *cmd)
{
	int	fd;
	char *path;
	char **args;

    cmd->pid_count++;
	pid_t	pid = fork();
	if (pid == 0)
	{
		fd = open(cmd->array[pos + 1], O_RDONLY);
		if (fd == -1)
		{
			perror("open failed for input file");
			exit(1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
		
		path = find_executable(get_first_word(ft_strdup(cmd->array_redirect[pos])), &(cmd->g_env_list));
		args = get_args(cmd->array_redirect[pos]);
		if (execve(path, args, cmd->envl) == -1)
		{
			free(path);
			exit(EXIT_FAILURE);
		}
		
	}
}


















void	execute_redirect_(int pos,  t_cmd *cmd)
{
	if (cmd->array[pos])
	{
		if (ft_strcmp(cmd->array_redirect[pos], ">") == 0)
		{
			ft_putstr_fd("\nAQUI executa: ", 2);
			ft_putstr_fd(cmd->array_redirect[pos], 2);
			ft_putchar_fd('\n', 2);
			exec_redout_(pos, cmd);
		}
/*		else if (ft_strcmp(cmd->array_redirect[pos], "<") == 0)
			exec_redin_(pos, cmd);
		else if (ft_strcmp(node->operator, ">>") == 0)
			exec_redout_append_(node, env, cmd);
		else if (ft_strcmp(node->operator, "<<") == 0)
			exec_heredoc_(node, env, cmd);
		else
			fprintf(stderr, "Erro: Operador desconhecido '%s'\n", node->operator);
	*/
	}
}










































