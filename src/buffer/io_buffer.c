/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_buffer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 14:29:25 by manandre          #+#    #+#             */
/*   Updated: 2024/10/23 14:29:26 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	gnc_buffer(t_cmd_line *cmd);

char	gnc_buffer(t_cmd_line *cmd)
{
	static int i;
	
	//printf("gnc_buffer:                    %d\n", i);
	char	c;

	c = cmd->buffer[cmd->pointer];
	add_pointer(cmd);
	cmd->lexema = ft_concat_char(cmd->lexema, c);
	i++;
	return (c);
}
