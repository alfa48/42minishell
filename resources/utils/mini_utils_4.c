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
    // Ponteiro para o início da palavra
    char *start = line;

    // Ignora espaços iniciais
    while (*start && isspace(*start)) {
        start++;
    }

    // Se a string estiver vazia ou composta apenas por espaços, retorna NULL
    if (*start == '\0') {
        return NULL;
    }

    // Ponteiro para o final da palavra
    char *end = start;

    // Avança até o primeiro espaço, tabulação ou fim da string
    while (*end && !isspace(*end)) {
        end++;
    }

    // Adiciona o terminador nulo à palavra
    *end = '\0';

    return start;
}
