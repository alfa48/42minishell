/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 09:37:53 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/29 13:52:43 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_array(char **array)
{
    if (array == NULL) {
        return;
    }

    size_t i = 0;
    while (array[i] != NULL)
	{
        free(array[i]);
        i++;
    }

    free(array);
}

int	is_redirect_char(char c)
{
	return (c == '<' || c == '>');
}
