/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_right.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 11:32:54 by manandre          #+#    #+#             */
/*   Updated: 2025/01/21 15:06:06 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*aux_heredoc_right(char *cmd, int pipefd[2])
{
	char	*heredoc_delim;

	(void)pipefd;
	// Obtém o delimitador do heredoc
	heredoc_delim = get_heredoc_delimiter(cmd);
	if (heredoc_delim)
	{
		// Usa a função passada como parâmetro para tratar o heredoc
		handle_heredoc_left(heredoc_delim, pipefd);
		// Libera o delimitador e ajusta o comando
		free(heredoc_delim);
		cmd = remove_heredoc(cmd);
	}
	return (cmd);
}

void	execute_pipe_right(int pos, t_cmd *cmd)
{
	int pid;
	char *path;
	char **args;
	t_redirect **redirects = NULL;
	char *var_str = NULL;

	cmd->pid_count++;
	pid = fork();
	if (pid == -1)
		return ;

	if (pid == 0)
	{
		var_str = aux_heredoc_right(cmd->array[pos], cmd->pipefd);
		redirects = parse_redirects(var_str);
		setup_io(redirects, NULL, cmd->pipefd, false);
		close(cmd->pipefd[0]);
		close(cmd->pipefd[1]);

		char *clean_cmd = remove_redirects(var_str);
		path = find_executable(get_first_word(ft_strdup(clean_cmd)),
				&(cmd->g_env_list));
		args = get_args(clean_cmd);

		free(clean_cmd);
		if (redirects)
			free_redirects(redirects);

		if (execve(path, args, cmd->envl) == -1)
			error_execve(args[0], path, args);
	}
}
