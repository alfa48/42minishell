/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_commands.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 11:23:19 by manandre          #+#    #+#             */
/*   Updated: 2025/01/30 20:32:30 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Função para restaurar e limpar file descriptors
void	cleanup_fds(int saved_st_in_out[2])
{
	if (saved_st_in_out[0] != -1)
		close(saved_st_in_out[0]);
	if (saved_st_in_out[1] != -1)
		close(saved_st_in_out[1]);
}

void	execute_child_process(t_redirect **redirects, t_cmd *cmd)
{
	int			opened_fds[MAX_REDIRECTS * 2];
	char		*path;
	char		**args;
	t_fd_data	fd_data;

	path = NULL;
	args = NULL;
	fd_data.fd_count = 0;
	fd_data.opened_fds = opened_fds;
	if (!apply_redirects(redirects, fd_data.opened_fds, &(fd_data.fd_count)))
		exit_child_process(cmd, path, args, &fd_data);
	path = find_executable(get_first_word(ft_strdup(cmd->clean_cmd)),
			&(cmd->g_env_list));
	if (!path)
	{
		cmd_not_found(get_first_word(ft_strdup(cmd->clean_cmd)));
		exit_child_process(cmd, path, args, &fd_data);
	}
	args = get_args(cmd->clean_cmd);
	if (!args)
		exit_child_process(cmd, path, args, &fd_data);
	execve(path, args, cmd->envl);
	perror("execve failed");
	exit_child_process(cmd, path, args, &fd_data);
}

// Função para lidar com o processo pai
void	execute_parent_process(int pid, t_cmd *cmd, int saved_st_in_out[2],
		char *clean_cmd)
{
	int	status;

	waitpid(pid, &status, 0);
	if (saved_st_in_out[0] != -1)
	{
		dup2(saved_st_in_out[0], STDIN_FILENO);
		close(saved_st_in_out[0]);
	}
	if (saved_st_in_out[1] != -1)
	{
		dup2(saved_st_in_out[1], STDOUT_FILENO);
		close(saved_st_in_out[1]);
	}
	free(clean_cmd);
	free_redirects(cmd->redirects);
	if (WIFEXITED(status))
		cmd->status_cmd = WEXITSTATUS(status);
}

int	free_src(t_cmd *cmd, int fds[2], char *clean_cmd, char *str)
{
	if (ft_strcmp(str, "redirects") == 0)
	{
		cleanup_fds(fds);
		return (1);
	}
	else if (ft_strcmp(str, "clean_cmd") == 0)
	{
		free_redirects(cmd->redirects);
		cleanup_fds(fds);
		return (1);
	}
	else if (ft_strcmp(str, "fork failed") == 0)
	{
		perror("fork failed");
		free(clean_cmd);
		free_redirects(cmd->redirects);
		cleanup_fds(fds);
		return (1);
	}
	return (0);
}
