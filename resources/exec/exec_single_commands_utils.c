/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_commands_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:30:10 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/30 18:51:09 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	handle_red_stdin(t_redirect **redirects)
{
	int	i;

	i = 0;
	while (redirects && redirects[i])
	{
		if (ft_strcmp(redirects[i]->type, "<") == 0)
		{
			redirects[i]->fd = open(redirects[i]->file, O_RDONLY);
			if (redirects[i]->fd != -1)
				dup2(redirects[i]->fd, STDIN_FILENO);
			else
			{
				perror("Error to open stdin file");
				exit(EXIT_FAILURE);
			}
		}
		i++;
	}
}

void	handle_redirects(t_redirect **redirects)
{
	int	i;

	i = 0;
	while (redirects && redirects[i])
	{
		if (ft_strcmp(redirects[i]->type, ">") == 0)
		{
			redirects[i]->fd = open(redirects[i]->file,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (redirects[i]->fd != -1)
				dup2(redirects[i]->fd, STDOUT_FILENO);
		}
		else if (ft_strcmp(redirects[i]->type, ">>") == 0)
		{
			redirects[i]->fd = open(redirects[i]->file,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (redirects[i]->fd != -1)
				dup2(redirects[i]->fd, STDOUT_FILENO);
		}
		i++;
	}
	handle_red_stdin(redirects);
}
