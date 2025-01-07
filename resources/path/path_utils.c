/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 21:09:35 by manandre          #+#    #+#             */
/*   Updated: 2025/01/06 21:09:37 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *find_executable(const char *command, t_env_var **g_env_list)
{
    (void)(g_env_list);
    char *path_env = get_env_var("PATH", *g_env_list);
    if (!path_env) {
        fprintf(stderr, "Erro: variável PATH não encontrada.\n");
        return NULL;
    }
    char *path = strdup(path_env);
    if (!path) {
        perror("strdup");
        return NULL;
    }
    char *token = strtok(path, ":"); // Divide o PATH em diretórios
    while (token) {
        // Constrói o caminho completo para o comando
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", token, command);

        // Verifica se o executável existe e é acessível
        if (access(full_path, X_OK) == 0) {
            free(path);
            return strdup(full_path); // Retorna o caminho completo do executável
        }

        token = strtok(NULL, ":"); // Próximo diretório
    }

    free(path);
    return NULL; // Não encontrado
}