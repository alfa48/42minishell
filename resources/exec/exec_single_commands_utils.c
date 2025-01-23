/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_commands_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:30:10 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/23 16:17:57 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Função para limpar e sair do processo filho
void	exit_child_process(t_cmd *cmd, char *path, char **args,
		t_fd_data *fd_data)
{
	int	i;

	i = 0;
	while (i < fd_data->fd_count)
	{
		if (fd_data->opened_fds[i] != -1)
			close(fd_data->opened_fds[i]);
		i++;
	}
	free(cmd->clean_cmd);
	free(path);
	if (args)
		free_array(args);
	free_redirects(cmd->redirects);
	exit(EXIT_FAILURE);
}

// Função para aplicar redirecionamentos
int	apply_redirects(t_redirect **redirects, int *opened_fds, int *fd_count)
{
	int	new_fd;
	int	i;

	i = -1;
	while (redirects[++i])
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
		opened_fds[(*fd_count)++] = new_fd;
		if ((ft_strcmp(redirects[i]->type, "<") == 0 && dup2(new_fd,
					STDIN_FILENO) == -1) || ((ft_strcmp(redirects[i]->type,
						">") == 0 || ft_strcmp(redirects[i]->type, ">>") == 0)
				&& dup2(new_fd, STDOUT_FILENO) == -1))
			return (0);
	}
	return (1);
}
