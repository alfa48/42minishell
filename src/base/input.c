/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 14:22:39 by manandre          #+#    #+#             */
/*   Updated: 2024/10/23 14:22:41 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tread_fwrite(t_cmd_line *cmd);

void	tread_fwrite(t_cmd_line *cmd)
{
	char	*input;

	cmd->fd = open("files/out.txt", FILE_MODE_WRITE, 0644);
	if (cmd->fd < 0)
		return ;
	input = readline("minishel$> ");
	if (input)
	{
		write(cmd->fd, input, ft_strlen(input));
	//	write(cmd->fd, "\n", 1);
		free(input);
	}
	close(cmd->fd);
}
