/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 11:40:56 by manandre          #+#    #+#             */
/*   Updated: 2025/01/21 11:50:59 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Função que verifica se há redirecionamento de saída
bool	has_output_redirect(t_redirect **redirects)
{
	int	i;

	if (!redirects)
		return (false);
	i = 0;
	while (redirects[i])
	{
		if (ft_strcmp(redirects[i]->type, ">") == 0 || 
			ft_strcmp(redirects[i]->type, ">>") == 0)
			return (true);	
		i ++;
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
		i ++;	
	}
	return (false);
}

// Helper function to handle both pipes and redirects
void setup_io(t_redirect **redirects, int *prev_pipe, int *next_pipe, bool is_middle)
{
    (void)is_middle;
    int i = 0;
    if (redirects) {
        // Apply file redirections first (they take precedence over pipes)
        while (redirects[i++]) {
            if (ft_strcmp(redirects[i]->type, "<") == 0) {
                redirects[i]->fd = open(redirects[i]->file, O_RDONLY);
                if (redirects[i]->fd != -1)
                    dup2(redirects[i]->fd, STDIN_FILENO);
            } else if (ft_strcmp(redirects[i]->type, ">") == 0) {
                redirects[i]->fd = open(redirects[i]->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (redirects[i]->fd != -1)
                    dup2(redirects[i]->fd, STDOUT_FILENO);
            } else if (ft_strcmp(redirects[i]->type, ">>") == 0) {
                redirects[i]->fd = open(redirects[i]->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (redirects[i]->fd != -1)
                    dup2(redirects[i]->fd, STDOUT_FILENO);
            }
        }
        printf("DEBUG: %i\n", i);
    }

    // Then handle pipes if no conflicting redirections exist
    if (!has_input_redirect(redirects) && prev_pipe && prev_pipe[0] != -1)
        dup2(prev_pipe[0], STDIN_FILENO);

    if (!has_output_redirect(redirects) && next_pipe && next_pipe[1] != -1)
        dup2(next_pipe[1], STDOUT_FILENO);
}