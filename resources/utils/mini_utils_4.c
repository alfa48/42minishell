/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils_4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:13:10 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/20 14:46:50 by fjilaias         ###   ########.fr       */
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
    ft_putstr_fd(str, 2);
    ft_putstr_fd(": command not found\n", 2);
}

int is_within_quotes(char *str, char *needed)
{
    int in_single_quote = 0;
    int in_double_quote = 0;
    int i = 0;
    char *found;

    if (!str || !needed || strlen(needed) == 0) // Verifica entradas inválidas
        return 0;

    // Busca a substring `needed` na string `str`
    found = strstr(str, needed);
    if (!found) // Se não encontrou a substring, retorna 0
        return 0;

    // Percorre a string até o início da substring encontrada
    while (&str[i] != found) // Usa ponteiros para comparar posições
    {
        if (str[i] == '\'' && !in_double_quote)
            in_single_quote = !in_single_quote; // Alterna estado de aspas simples
        else if (str[i] == '\"' && !in_single_quote)
            in_double_quote = !in_double_quote; // Alterna estado de aspas duplas
        i++;
    }

    // Retorna se a posição inicial da substring está dentro de aspas
    return (in_single_quote || in_double_quote);
}



int is_entirely_within_quotes(char *str)
{
    if ((str[0] == '\'' && str[strlen(str) - 1] == '\'') ||
        (str[0] == '\"' && str[strlen(str) - 1] == '\"'))
        return (1);
    return (0);
}

