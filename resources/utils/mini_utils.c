/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:24:48 by manandre          #+#    #+#             */
/*   Updated: 2024/12/12 16:37:59 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_tree(t_node *root, char **env, t_cmd *cmd);

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
	execve_args[0] =  ft_strjoin("/bin/", cmd_args[0]);
	while (++i < get_args_size(cmd_args))
		execve_args[i] = cmd_args[i];
	execve_args[i] = NULL;
	return (execve_args);
}

void	execute_command(t_node *node, char **env)
{
	char	**execve_args;

	pid_t pid = fork();
	if (pid == 0)
	{
		execve_args = get_args(node->command);
		fprintf(stderr, "Debug: Comando full %s\n", execve_args[0]);
		execve(execve_args[0], execve_args, env);
		perror("execve failed");
		exit(1);
	}
	//waitpid(pid, NULL, 0);
}

void	execute_pipe(t_node *node, char **env, t_cmd *cmd)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe failed");
		return ;
	}
	pid_t pid1 = fork();
	if (pid1 == 0)
	{
		// Processo filho para o comando da esquerda
		dup2(pipefd[1], STDOUT_FILENO);  // Redireciona stdout para o pipe
		close(pipefd[0]);
		close(pipefd[1]);
		execute_tree(node->left, env, cmd);
		exit(0);
	}
	waitpid(pid1, NULL, 0);
	pid_t pid2 = fork();
	if (pid2 == 0)
	{
		// Processo filho para o comando da direita
		dup2(pipefd[0], STDIN_FILENO);   // Redireciona stdin para o pipe
		close(pipefd[0]);
		close(pipefd[1]);
		execute_tree(node->right, env, cmd);
		exit(0);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid2, NULL, 0);
}

void	exec_redout(t_node *node, char **env,  t_cmd *cmd)
{
	int	fd;

	pid_t pid = fork();
	if (pid == 0)
	{
		fd = open(node->right->command, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd != -1)
		{
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		execute_tree(node->left, env, cmd);
		exit(0);
	}
	waitpid(pid, NULL, 0);
}
/*
       <<
       
command   EOF | fn.txt
*/
void	exec_redin(t_node *node, char **env,  t_cmd *cmd)
{
	int	fd;

	pid_t pid = fork();
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
		execute_tree(node->left, env, cmd);
		exit(0);
	}
	waitpid(pid, NULL, 0);
}

void	exec_redout_append(t_node *node, char **env,  t_cmd *cmd)
{
	int	fd;

	pid_t pid = fork();
	if (pid == 0)
	{
		fd = open(node->right->command, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			perror("open failed for output file");
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		execute_tree(node->left, env, cmd);
		exit(0);
	}
	waitpid(pid, NULL, 0);
}

void	exec_heredoc(t_node *node, char **env,  t_cmd *cmd)
{
    char* result = NULL;
    size_t total_size = 0;
    size_t buffer_size = 1024;
    int	fd;
   pid_t	pid;
    result = malloc(buffer_size);
    result[0] = '\0';\
    
    pid = fork();
    if (pid == 0)
    {
    		while (1)
	    {

		char* line = readline("heredoc> ");
		if (!line) {
		printf("Apagando %s\n", line);
		    free(result);
		    return ;
		}
		// Verifica se é o marcador de fim
		if (strcmp(line, node->right->command) == 0) {
		    free(line);
		    break;
		}
		// Calcula novo tamanho necessário
		size_t needed = total_size + strlen(line) + 2; // +2 para \n e \0
		// Realoca se necessário
		if (needed > buffer_size) {
		    buffer_size = needed * 2;
		    char* new_buffer = realloc(result, buffer_size);
		    if (!new_buffer) {
		        free(result);
		        free(line);
		        return ;
		    }
		    result = new_buffer;
		}
		// Adiciona a linha ao resultado
		strcat(result, line);
		strcat(result, "\n");
		total_size = strlen(result);
		// Limpa a linha atual e move para nova linha
		rl_replace_line("", 0);
		//rl_on_new_line();
		rl_redisplay();
		free(line);
		
		// Redirecionar stdin para o terminal
		int fd_terminal = open("/dev/tty", O_RDONLY);
		if (fd_terminal == -1) {
		perror("Erro ao abrir /dev/tty");
		exit(1);
		}
		dup2(fd_terminal, STDIN_FILENO);
		close(fd_terminal);
	    }
	    fd = open("heredoc_file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	    write(fd, result, ft_strlen(result));
	    free(result);
	    free(node->right->command);
	    node->right->command = ft_strdup("heredoc_file.txt");
	    	exec_redin(node, env, cmd);
	}
	waitpid(pid, NULL, 0);
}

/*

cat << kjhk

    <<
cmd     hdoc> f.txt


*/

void	ffexec_heredoc(t_node *node, char **env,  t_cmd *cmd)
{
	char	*line;
	size_t		len;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe failed");
		return ;
	}
	pid_t pid = fork();
	if (pid == 0)
	{
		close(pipefd[0]); // Fechar o lado de leitura no processo filho
		line = NULL;
		len = 0;
		while (1)
		{
			fprintf(stderr, "> "); // Prompt do here-doc
			if (getline(&line, &len, stdin) == -1)
			{
				perror("getline failed");
				break ;
			}
			if (strncmp(line, node->right->command, strlen(node->right->command)) == 0 &&
				line[strlen(node->right->command)] == '\n')
				break ; // Se o delimitador for encontrado
			write(pipefd[1], line, strlen(line));
		}
		free(line);
		close(pipefd[1]);
		exit(0);
	}
	waitpid(pid, NULL, 0);
	close(pipefd[1]); // Fechar o lado de escrita no processo pai
	pid = fork();
	if (pid == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		execute_tree(node->left, env, cmd);
		exit(0);
	}
	close(pipefd[0]);
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

void	execute_tree(t_node *root, char **env, t_cmd *cmd)
{
	if (!root)
		return ;
	fprintf(stderr, "Debug: Executando comando %s\n", root->command ? root->command : root->operator);
	if (root->command != NULL)
	{
		execute_command(root, env);
		cmd->ncmd++;
	}
	else if (root->operator && ft_strcmp(root->operator, "|") == 0)
		execute_pipe(root, env, cmd);
	else if (root->operator != NULL)
		execute_redirect(root, env, cmd);
		
	
}

void    exec(t_cmd *cmd, char **env)
{
	int	i = -1;
	t_node *tmp_root = cmd->root;
	(void) env;
	fprintf(stderr, "Debug: root: %s\n", cmd->root->operator);
	execute_tree(tmp_root, env, cmd);
	while (++i < cmd->ncmd)
		waitpid(-1, NULL, 0);
}
