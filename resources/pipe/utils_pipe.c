/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 11:40:56 by manandre          #+#    #+#             */
/*   Updated: 2025/01/22 11:30:48 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_execve(char *ccmd, char *path, char **args)
{
	cmd_not_found(ccmd);
	free(path);
	free_array(args);
	exit(1);
}

// Função que verifica se há redirecionamento de saída
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

// Helper function to handle both pipes and redirects
void	setup_io(t_redirect **redirects, int *prev_pipe, int *next_pipe,
		bool is_middle)
{
	int	i;

	(void)is_middle;
	i = 0;
	if (redirects)
	{
		while (redirects[i++])
		{
			if (ft_strcmp(redirects[i]->type, "<") == 0)
			{
				redirects[i]->fd = open(redirects[i]->file, O_RDONLY);
				if (redirects[i]->fd != -1)
					dup2(redirects[i]->fd, STDIN_FILENO);
			}
			else if (ft_strcmp(redirects[i]->type, ">") == 0)
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
		}
		printf("DEBUG: %i\n", i);
	}
	// Then handle pipes if no conflicting redirections exist
	if (!has_input_redirect(redirects) && prev_pipe && prev_pipe[0] != -1)
		dup2(prev_pipe[0], STDIN_FILENO);
	if (!has_output_redirect(redirects) && next_pipe && next_pipe[1] != -1)
		dup2(next_pipe[1], STDOUT_FILENO);
}
void	execute_with_args(char *clean_cmd, t_redirect **redirects, t_cmd *cmd)
{
	char	*path;
	char	**args;

	path = find_executable(get_first_word(ft_strdup(clean_cmd)),
			&(cmd->g_env_list));
	args = get_args(clean_cmd);
	free(clean_cmd);
	if (redirects)
		free_redirects(redirects);
	if (execve(path, args, cmd->envl) == -1)
		error_execve(args[0], path, args);
}
