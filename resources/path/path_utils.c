/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 21:09:35 by manandre          #+#    #+#             */
/*   Updated: 2025/01/17 11:05:49 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_executable(const char *command, t_env_var **g_env_list)
{
	(void)(g_env_list);
	char	full_path[1024];
	char	*path_env;
	char	*path;
	char	*token;

	if (command[0] == '.' || command[0] == '/')
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		return (NULL);
	}
	path_env = get_env_var("PATH", *g_env_list);
	if (!path_env)
	{
		ft_putstr_fd("Erro: variável PATH não encontrada.\n", 2);
		return (NULL);
	}
	path = ft_strdup(path_env);
	if (!path)
	{
		perror("strdup");
		return (NULL);
	}
	token = strtok(path, ":"); // Divide o PATH em diretórios
	while (token)
	{
        // Constrói o caminho completo para o comando
        snprintf(full_path, sizeof(full_path), "%s/%s", token, command);
        // Verifica se o executável existe e é acessível
		if (access(full_path, X_OK) == 0)
		{
        		free(path);
        		return (ft_strdup(full_path)); // Retorna o caminho completo do executável
		}
        token = strtok(NULL, ":"); // Próximo diretório
	}
	free(path);
	return (NULL); // Não encontrado
}
