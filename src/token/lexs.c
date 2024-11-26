/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 14:42:14 by manandre          #+#    #+#             */
/*   Updated: 2024/11/07 15:05:45 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_clear(t_cmd_line *cmd);
void	ft_enter(t_cmd_line *cmd);

void	ft_clear(t_cmd_line *cmd)
{
//	printf("clear ini\n");
	cmd->pointer = cmd->init_lex;
	free(cmd->lexema);
	cmd->lexema = ft_strdup("");
//	printf("clear fim\n");
}

void	ft_enter(t_cmd_line *cmd)
{
//	printf("enter ini\n");
	cmd->init_lex = cmd->pointer;
	free(cmd->lexema);
	cmd->lexema = ft_strdup("");
//	printf("enter fim\n");
}