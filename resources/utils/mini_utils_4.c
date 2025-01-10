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

// Função para buscar o valor de uma variável de ambiente na lista
char *get_env_var(const char *name, t_env_var *env_list)
{
    while (env_list)
    {
        // Compara o nome da variável com o nome procurado
        if (strcmp(env_list->name, name) == 0)
        {
            return env_list->value; // Retorna o valor se encontrar
        }
        env_list = env_list->next; // Avança para o próximo nó
    }
    return NULL; // Retorna NULL se não encontrar a variável
}

char *get_first_word(char *line)
{
    while (*line && isspace(*line))
        line++;
    if (*line == '\0')
        return NULL;
    char *end = line;
    while (*end && !isspace(*end)) {
        end++;
    }
    *end = '\0';
    return line;
}

void cmd_not_found(char *str)
{
    printf("%s: command not found\n", str);
}

