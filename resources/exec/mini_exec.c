/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 08:32:29 by manandre          #+#    #+#             */
/*   Updated: 2025/01/21 08:53:32 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_single_command(char *cmd_str, t_cmd *cmd)
{
	int			pid;
	char		*path;
	char		**args;
	t_redirect	**redirects;
	char		*clean_cmd;
	int			saved_stdin;
	int			saved_stdout;
	int			opened_fds[MAX_REDIRECTS * 2];
	int			fd_count;
	int			new_fd;
	int			status;

	path = NULL;
	args = NULL;
	redirects = NULL;
	clean_cmd = NULL;
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (!cmd_str || !cmd)
	{
		if (saved_stdin != -1)
			close(saved_stdin);
		if (saved_stdout != -1)
			close(saved_stdout);
		return ;
	}
	// Parse os redirecionamentos do comando
	redirects = parse_redirects(cmd_str);
	if (!redirects)
	{
		close(saved_stdin);
		close(saved_stdout);
		return ;
	}
	// Obtém o comando limpo antes do fork
	clean_cmd = remove_redirects(cmd_str);
	if (!clean_cmd)
	{
		free_redirects(redirects);
		close(saved_stdin);
		close(saved_stdout);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		free(clean_cmd);
		free_redirects(redirects);
		close(saved_stdin);
		close(saved_stdout);
		return ;
	}
	if (pid == 0) // Processo filho
	{
		fd_count = 0;
		// Aplica os redirecionamentos na ordem correta
		for (int i = 0; redirects[i]; i++)
		{
			new_fd = -1;
			if (ft_strcmp(redirects[i]->type, "<") == 0)
				new_fd = open(redirects[i]->file, O_RDONLY);
			else if (ft_strcmp(redirects[i]->type, ">") == 0)
				new_fd = open(redirects[i]->file, O_WRONLY | O_CREAT | O_TRUNC,
						0644);
			else if (ft_strcmp(redirects[i]->type, ">>") == 0)
				new_fd = open(redirects[i]->file, O_WRONLY | O_CREAT | O_APPEND,
						0644);
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
		path = find_executable(get_first_word(ft_strdup(clean_cmd)),
				&(cmd->g_env_list));
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

void	exec(t_cmd *cmd)
{
	pipe(cmd->pipefd);
	if (ft_strchr(cmd->line1, PIPE))
		execute_commands(cmd->size, cmd);
	else if (has_redirect(cmd->line1))
		execute_single_command(cmd->line1, cmd);
	close(cmd->pipefd[0]);
	close(cmd->pipefd[1]);
	wait_forks(cmd);
}