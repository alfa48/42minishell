/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_buffer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 14:27:10 by manandre          #+#    #+#             */
/*   Updated: 2024/11/01 09:41:40 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_buffers(t_cmd_line *cmd);
void	add_pointer(t_cmd_line *cmd);
void	gpc_buffer(t_cmd_line *cmd);
void	load_buffer1(t_cmd_line *cmd);
void    load_buffer2(t_cmd_line *cmd);

void	init_buffers(t_cmd_line *cmd)
{
	cmd->corrent_buffer = 2;
	cmd->lexema = ft_strdup("");
	cmd->init_lex = 0;
	cmd->buffer = malloc(sizeof(char) * (BUFFER_SIZE * 2));
	cmd->pointer = 0;
	cmd->fd = open_fread();
	load_buffer1(cmd);
}

void	add_pointer(t_cmd_line *cmd)
{
	cmd->pointer++;
	if (cmd->pointer == BUFFER_SIZE)
		load_buffer2(cmd);
	else if (cmd->pointer == (BUFFER_SIZE * 2))
	{
		load_buffer1(cmd);
		cmd->pointer = 0;
	}
}

void	gpc_buffer(t_cmd_line *cmd)
{
	//static int i;	
	
	//printf("gpc_buffer:                    %d\n", i);
	cmd->pointer--;
	cmd->lexema = rm_last_char(cmd->lexema);
	if (cmd->pointer < 0)
		cmd->pointer = (BUFFER_SIZE * 2) - 1;
	//i++;
}

void	load_buffer1(t_cmd_line *cmd)
{
	int	i;

	if(cmd->corrent_buffer == 2)
	{
		cmd->corrent_buffer = 1;
		ft_memset(cmd->buffer, '\0', BUFFER_SIZE);
		i = 0;
		//printf("CARREGAR BUFER 1\n");
		while (i < BUFFER_SIZE)
		{
			cmd->buffer[i] = gnc(cmd->fd);
			//printf("load_buffer1 char %d: %c\n", i, cmd->buffer[i]);
			if (cmd->buffer[i] == '\0')
				break;
			i++;
		}
	}
}

void    load_buffer2(t_cmd_line *cmd)
{
        int     i;
        char	c;

	if (cmd->corrent_buffer == 1)
	{
		cmd->corrent_buffer = 2;
		ft_memset(cmd->buffer + BUFFER_SIZE, '\0', BUFFER_SIZE * 2);
        	i = BUFFER_SIZE;
		//printf("CARREGAR BUFER 2\n");
	        while (i < (BUFFER_SIZE * 2))
		{
			c = gnc(cmd->fd);
			
			//printf("load_buffer2 char %d: %c (%d)\n", i, c, c);
			cmd->buffer[i] = c;
			//printf("load_buffer2 char %d: %c\n", i, cmd->buffer[i]);
			if (cmd->buffer[i] == '\0')
			{
				//printf("load_buffer2 NULL - EOF or empty character detected\n");
				break;
			}
			i++;
		}
	}
}
