/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 21:09:35 by manandre          #+#    #+#             */
/*   Updated: 2025/01/30 23:29:45 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*construct_full_path(const char *directory, const char *command)
{
	char	*full_path;
	int		length;

	length = ft_strlen(directory) + ft_strlen(command) + 2;
	full_path = malloc(length);
	if (full_path == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	ft_strlcpy(full_path, directory, ft_strlen(directory) + 1);
	mini_strcat(full_path, "/");
	mini_strcat(full_path, command);
	return (full_path);
}

char	*find_in_directories(const char *command, char **directories)
{
	char	*full_path;
	int		i;

	i = 0;
	while (directories[i] != NULL)
	{
		full_path = construct_full_path(directories[i], command);
		if (full_path == NULL)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_executable(const char *command, t_env_var **g_env_list)
{
	char	*path_env;
	char	**directories;
	char	*result;

	if (command[0] == '.' || command[0] == '/')
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		return (NULL);
	}
	path_env = get_env_var("PATH", *g_env_list);
	if (!path_env)
		return (NULL);
	directories = ft_split(path_env, ':');
	if (!directories)
	{
		perror("ft_split");
		return (NULL);
	}
	result = find_in_directories(command, directories);
	free_array(directories);
	return (result);
}
