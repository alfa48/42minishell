/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_envexit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:17:02 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/07 10:19:42 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    mini_exit(t_cmd *cmd)
{
	(void)cmd;
	char *str;

	ft_putstr_fd("exit\n", 1);
	if (cmd->arg[1])
	{
		str = cmd->arg[1];
		while (*str)
		{
			if (ft_isalpha(*str))
			{
			    printf("minishell: exit: %s: numeric argument required\n", cmd->arg[1]);
				exit(0);
			}
			str++;
		}
		if (cmd->arg[2])
			printf("minishell: exit: too many arguments\n");	
	}

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
