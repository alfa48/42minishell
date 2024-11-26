/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 09:32:36 by manandre          #+#    #+#             */
/*   Updated: 2024/11/15 09:09:16 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_space(t_cmd_line *cmd)
{
	char tmp;

	tmp = gnc_buffer(cmd);
	while (tmp == ' ')
		tmp = gnc_buffer(cmd);
	gpc_buffer(cmd);
}

t_token *check_pipe_or(t_cmd_line *cmd)
{
	char tmp;

	tmp = gnc_buffer(cmd);
	if (tmp == '|')
	{
		tmp = gnc_buffer(cmd);
		if (tmp == '|')
		{
			return (create_token(CONDITIONAL_OR, "||"));
		}
		gpc_buffer(cmd);
		return (create_token(PIPE, "|"));
	}
	gpc_buffer(cmd);
	return (NULL);
}
