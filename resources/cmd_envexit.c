/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_envexit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:17:02 by fjilaias          #+#    #+#             */
/*   Updated: 2024/12/10 11:30:08 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    mini_exit(void)
{
    exit(0);
}

void    mini_env(t_env_var *g_env_list) 
{
	t_env_var *current;

    current = g_env_list;
    while (current)
    {
        printf("%s=%s\n", current->name, current->value);
        current = current->next;
    }
}

void	free_ms(t_cmd *cmd)
{
	int	i;

	i = -1;
	if (cmd->arg)
	{
		while (cmd->arg[++i])
			free(cmd->arg[i]);
		free(cmd->arg);
		cmd->arg = NULL;
	}
}
