/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils_4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:13:10 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/06 15:20:37 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *does_it_exist(char *name, t_env_var *current)
{
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
            printf("O valor da variável %s é %s", name, current->value);
			return (current->value);
		}
		current = current->next;
    }
	return (NULL);
}
