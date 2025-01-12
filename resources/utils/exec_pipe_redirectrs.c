#include "minishell.h"

// void	execute_pipe(t_node *node, char **env, t_cmd *cmd)
// {
// 	(void) env;
// 	int	pipefd[2];

// 	if (pipe(pipefd) == -1)
// 	{
// 		perror("pipe failed");
// 		return ;
// 	}
// 	pid_t	pid1 = fork();
// 	if (pid1 == 0)
// 	{
// 		// Processo filho para o comando da esquerda
// 		dup2(pipefd[1], STDOUT_FILENO);  // Redireciona stdout para o pipe
// 		close(pipefd[0]);
// 		close(pipefd[1]);
// 		fprintf(stderr, "Debug: Executando comando da esquerda: %s\n", node->left->command);
// 		fork_exec_cmd(cmd, node->left);
// 		//execute_tree(node->left, cmd);
// 		exit(0);
// 	}
// 	waitpid(pid1, NULL, 0);
// 	pid_t	pid2 = fork();
// 	if (pid2 == 0)
// 	{
// 		// Processo filho para o comando da direita
// 		dup2(pipefd[0], STDIN_FILENO);   // Redireciona stdin para o pipe
// 		close(pipefd[0]);
// 		close(pipefd[1]);
// 		fprintf(stderr, "Debug: Executando comando da direita: %s\n", node->right->command);
// 		fork_exec_cmd(cmd, node->left);
// 		//execute_tree(node->right, cmd);
// 		exit(0);
// 	}
// 	close(pipefd[0]);
// 	close(pipefd[1]);
// 	waitpid(pid2, NULL, 0);
// }

void	execute_pipe_right(int pos, t_cmd *cmd)
{
	int pid;
	char *path;
	char **args;

	cmd->pid_count++;
	pid = fork();
	if (pid == 0)
	{
//		cmd -> |
		path = find_executable(get_first_word(ft_strdup(cmd->array[pos])), &(cmd->g_env_list));

		args = get_args(cmd->array[pos]);

		dup2(cmd->pipefd[1], STDOUT_FILENO);  // Redireciona stdout para o pipe
		close(cmd->pipefd[0]);
		close(cmd->pipefd[1]);

		if (execve(path, args, cmd->envl) == -1)
		{
			cmd_not_found(get_first_word(ft_strdup(cmd->array[pos])));
			free(path);
			exit(1);
		}
	}
	//  else{
	//  	// Processo pai: fecha os descritores do pipe
	  	//close(cmd->pipefd[0]);
	  	//close(cmd->pipefd[1]);
	//  }
}

void	execute_pipe_left(int pos, t_cmd *cmd)
{
	int pid;
	char *path;
	char **args;

	cmd->pid_count++;
	pid = fork();
	if (pid == 0)
	{
//		| -> cmd
		dup2(cmd->pipefd[0], STDIN_FILENO);  // Redireciona stdout para o pipe
		close(cmd->pipefd[1]);
		close(cmd->pipefd[0]);
		path = find_executable(get_first_word(ft_strdup(cmd->array[pos])), &(cmd->g_env_list));
		args = get_args(cmd->array[pos]);
		if (execve(path, args, cmd->envl) == -1)
		{
			cmd_not_found(get_first_word(ft_strdup(cmd->array[pos])));
			free(path);
			exit(EXIT_FAILURE);
		}
	}
	 else{
	 	// Processo pai: fecha os descritores do pipe
	 	//close(cmd->pipefd[0]);
	 	//close(cmd->pipefd[1]);
	 }
}

void execute_pipe_middle(int pos, t_cmd *cmd)
{
    int pid;
    char *path;
    char **args;

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
        // Redireciona entrada padrão para o pipe de leitura
        dup2(cmd->prev_pipe[0], STDIN_FILENO);

        // Redireciona saída padrão para o pipe de escrita
        dup2(cmd->pipefd[1], STDOUT_FILENO);
        // Fecha os descritores do pipe (já redirecionados)
        // Fecha todos os descritores de pipe
        close(cmd->prev_pipe[0]);
        close(cmd->prev_pipe[1]);
        close(cmd->pipefd[0]);
        close(cmd->pipefd[1]);


        // Obtém o caminho do executável e os argumentos
        path = find_executable(get_first_word(ft_strdup(cmd->array[pos])), &(cmd->g_env_list));
        args = get_args(cmd->array[pos]);
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
}




























void	exec_redout_(int pos, t_cmd *cmd)
{
	int	fd;
	int status;

    cmd->pid_count++;
	pid_t	pid = fork();
	if (pid == 0)
	{
		fd = open(cmd->array[pos + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("open failed for output file");
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);// e' mesmo necessario usar o dup2 ou posso usar o dup
		close(fd);
		fork_exec_cmd_(pos - 1, cmd);
		waitpid(-1, &status, 0);
        if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 1)
					cmd_not_found(get_first_word(ft_strdup(cmd->array[pos - 1])));
            exit(WEXITSTATUS(status));
		}
		exit(-1);
	}
}

void	exec_redin_(int pos, t_cmd *cmd)
{
	int	fd;
	int status;

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
		fork_exec_cmd_(pos - 1, cmd);
		waitpid(-1, &status, 0);
        if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 1)
					cmd_not_found(get_first_word(ft_strdup(cmd->array[pos - 1])));
            exit(WEXITSTATUS(status));
		}
		exit(-1);
	}
}
/*
void	exec_redout_append_(int pos, t_cmd *cmd)
{
	int	fd;

	pid_t	pid = fork();
	if (pid == 0)
	{
		// Processo filho: abre o arquivo em modo de anexar
		fd = open(cmd->array[pos + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
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
*/




















void	execute_redirect_(int pos,  t_cmd *cmd)
{
	if (cmd->array[pos])
	{
		if (ft_strcmp(cmd->array[pos], ">") == 0)
			exec_redout_(pos, cmd);
		else if (ft_strcmp(cmd->array[pos], "<") == 0)
			exec_redin_(pos, cmd);
/*		else if (ft_strcmp(node->operator, ">>") == 0)
			exec_redout_append_(node, env, cmd);
		else if (ft_strcmp(node->operator, "<<") == 0)
			exec_heredoc_(node, env, cmd);
		else
			fprintf(stderr, "Erro: Operador desconhecido '%s'\n", node->operator);
	*/
	}
}