/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 09:08:22 by manandre          #+#    #+#             */
/*   Updated: 2025/01/17 16:27:26 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int checks_start_end(char *str)
{
    int rs;

    rs = 0;

    if (!str || !*str) // Verifica se a string é nula ou vazia
        return 0;

    char patterns[] = {'<', '>', '|'};
    int size = sizeof(patterns) / sizeof(patterns[0]);
    int i = 0;

    // Verifica o primeiro caractere
    while (i < size) {
        if (str[0] == patterns[i])
            rs = 1;
        i++;
    }

    // Encontrar o ultimo caractere não nulo
    i = 0;
    while (str[i])
        i++;
    i--; // Agora i aponta para o último caractere

    // Verifica o último caractere
    int j = 0;
    while (j < size) {
        if (str[i] == patterns[j])
            rs = 1;
        j++;
    }

    if (rs)
    {
        printf("bash: syntax error near unexpected token `newline'\n");
        return (1);
    }
    return 0; // Nenhum erro encontrado
}

int checks_error_pattern(char *str)
{
    int i = 0;
    char c;
    char    space;
    
    while (str[i])
    {
        space = 0;
        while (str[i] <= 32)
            i++;
        if (str[i] && (str[i] == '<' || str[i] == '>' || str[i] == '|' || str[i] == '&'))
        {
            c = str[i];
            while (str[++i] <= 32)
                space = 1;
            //c = str;
            //caso do heredoc
            if (!str[i])
                return (0);
            if (!space && ((c == '<' && str[i] == '<') || (c == '>' && str[i] == '>')))
                continue;
            if (str[i] == '|')
            {
                printf("bash: syntax error near unexpected token `|'\n");
                return (1);
                
            }
            else if (str[i] == '<' || str[i] == '>' || str[i] == '|')
            {
                printf("bash: syntax error near unexpected token `newline'\n");
                return (1);
            }
        }
     i++; 
    }
    
    return (0);
}


int checks_str(t_cmd *cmd)
{
    (void)cmd;
    char    *str;
    str = mini_epur_str(cmd->line);
    if (checks_start_end(str) || checks_error_pattern(str))
        return (1);
    free(str);
    return (0);
}
