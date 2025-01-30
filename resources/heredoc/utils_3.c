/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:08:05 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/30 10:04:18 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_heredoc(char *cmd)
{
	char	*heredoc_pos;
	char	*clean_cmd;

	heredoc_pos = mini_strstr(cmd, "<<");
	if (!heredoc_pos)
		return (ft_strdup(cmd));
	clean_cmd = malloc(sizeof(char) * (heredoc_pos - cmd + 1));
	if (!clean_cmd)
		return (NULL);
	ft_strlcpy(clean_cmd, cmd, heredoc_pos - cmd + 1);
	return (clean_cmd);
}
