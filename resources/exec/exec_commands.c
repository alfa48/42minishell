/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 11:21:36 by manandre          #+#    #+#             */
/*   Updated: 2025/01/30 20:24:40 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	mini_putstr_fd(char *str, int fd)
{
	int	c;

	c = 0;
	if (str == NULL)
		return (0);
	while (str[c] != '\0')
	{
		write(fd, &str[c], 1);
		c++;
	}
	return (c);
}

int	handle_pipe_exec(int pos, t_cmd *cmd)
{
	if (pos < cmd->size && cmd->array[pos + 1])
	{
		if (ft_strcmp(cmd->array[pos + 1], "|") == 0)
		{
			if (pos > 0 && cmd->array[pos - 1])
			{
				if (ft_strcmp(cmd->array[pos - 1], "|") == 0)
				{
					execute_pipe_middle_(pos, cmd);
					return (1);
				}
			}
			if (pipe(cmd->pipefd) == -1)
				return (0 * mini_putstr_fd("pipe failed", 2));
			execute_pipe_right(pos, cmd);
			return (1);
		}
	}
	else if (pos > 0 && cmd->array[pos - 1])
	{
		if (ft_strcmp(cmd->array[pos - 1], "|") == 0)
			execute_pipe_left(pos, cmd);
	}
	mini_close_fd(cmd->pipefd[0], cmd->pipefd[1]);
	return (1);
}

void	handle_redirect(int pos, t_cmd *cmd)
{
	if (pos > 0 && cmd->array[pos - 1])
	{
		if (is_redirect(cmd->array[pos - 1]))
			return ;
	}
}

void	handle_command_exec(int pos, t_cmd *cmd)
{
	if (cmd->array[pos] && !is_operator(cmd->array[pos]))
	{
		handle_pipe_exec(pos, cmd);
	}
	else if (cmd->array[pos])
		return ;
}

void	execute_commands(int pos, t_cmd *cmd)
{
	if (!cmd->array[pos])
		return ;
	execute_commands(pos - 1, cmd);
	handle_redirect(pos, cmd);
	handle_command_exec(pos, cmd);
}
