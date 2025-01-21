/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils_6.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 14:47:11 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/21 15:53:25 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_word(char *line, int *sig, char *siline)
{
	char	*init;

	init = mini_strstr(line, "echo");
	if (!init)
		return (NULL);
	init += 4;
	while (*init && *init <= 32)
		init++;
	if (!mini_strstr(init, "-n") || is_within_quotes(siline, "-n"))
	{
		while (*init && (*init == ' ' || *init == '\t'))
			init++;
		*sig = 1;
	}
	else
	{
		init += 2;
		while (*init && (*init == ' ' || *init == '\t'))
			init++;
	}
	return (init);
}
