/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 11:40:56 by manandre          #+#    #+#             */
/*   Updated: 2025/01/22 11:52:31 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	has_output_redirect(t_redirect **redirects)
{
	int	i;

	if (!redirects)
		return (false);
	i = 0;
	while (redirects[i])
	{
		if (ft_strcmp(redirects[i]->type, ">") == 0
			|| ft_strcmp(redirects[i]->type, ">>") == 0)
			return (true);
		i++;
	}
	return (false);
}

// Função que verifica se há redirecionamento de entrada
bool	has_input_redirect(t_redirect **redirects)
{
	int	i;

	i = 0;
	if (!redirects)
		return (false);
	while (redirects[i])
	{
		if (ft_strcmp(redirects[i]->type, "<") == 0)
			return (true);
		i++;
	}
	return (false);
}

void	apply_file_redirections(t_redirect **r)
{
	int	i;

	i = 0;
	while (r[i++])
	{
		if (ft_strcmp(r[i]->type, "<") == 0)
		{
			r[i]->fd = open(r[i]->file, O_RDONLY);
			if (r[i]->fd != -1)
				dup2(r[i]->fd, STDIN_FILENO);
		}
		else if (ft_strcmp(r[i]->type, ">") == 0)
		{
			r[i]->fd = open(r[i]->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (r[i]->fd != -1)
				dup2(r[i]->fd, STDOUT_FILENO);
		}
		else if (ft_strcmp(r[i]->type, ">>") == 0)
		{
			r[i]->fd = open(r[i]->file,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (r[i]->fd != -1)
				dup2(r[i]->fd, STDOUT_FILENO);
		}
	}
}

void	handle_pipes(t_redirect **redirects, int *prev_pipe, int *next_pipe)
{
	if (!has_input_redirect(redirects) && prev_pipe && prev_pipe[0] != -1)
		dup2(prev_pipe[0], STDIN_FILENO);
	if (!has_output_redirect(redirects) && next_pipe && next_pipe[1] != -1)
		dup2(next_pipe[1], STDOUT_FILENO);
}

void	setup_io(t_redirect **redirects, int *prev_pipe, int *next_pipe,
		bool is_middle)
{
	(void)is_middle;
	if (redirects)
	{
		apply_file_redirections(redirects);
	}
	handle_pipes(redirects, prev_pipe, next_pipe);
}
