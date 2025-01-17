/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 09:08:22 by manandre          #+#    #+#             */
/*   Updated: 2025/01/17 09:08:25 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
int checks_error_pattern(char *str)
{
    int i = 0;

    while (str[i] && str[i + 1])
        i++;
     // Verifica operadores inválidos no final da string
    if (str[i] && (str[i] == '<' || str[i] == '>' || str[i] == '|' || str[i] == '&'))
        return (1);
    return (0);
}

int checks_str(t_cmd *cmd)
{
    if (checks_error_pattern(cmd->line)) 
    {
        //exibe mensagem
        free(cmd->line);
        return (1);
    }
    return (0);
}
