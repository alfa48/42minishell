/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_envexit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:17:02 by fjilaias          #+#    #+#             */
/*   Updated: 2024/11/20 11:42:52 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    mini_exit(void)
{
    exit(0);
}

void    mini_env(t_env_var *g_env_list) 
{
    t_env_var *current = g_env_list;
    while (current)
    {
        printf("%s=%s\n", current->name, current->value);
        current = current->next;
    }
}

void free_ms(char** matriz)
{
	int	i;
	int	j;

	i = 0;
	if (matriz)
	{
		while (matriz[i])
			i ++;
		j = -1;
		while (++j < i)
			free(matriz[j]);
		free(matriz);
	}
}