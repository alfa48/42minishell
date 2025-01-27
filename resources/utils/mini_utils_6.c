/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils_6.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 14:47:11 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/27 10:57:27 by fjilaias         ###   ########.fr       */
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
	if ((mini_strstr(init, "-n") && *(mini_strstr(init, "-n") + 2) != ' ')
		&& !is_within_quotes(siline, "-n"))
		*sig = 2;
	else if (!mini_strstr(init, "-n") || is_within_quotes(siline, "-n"))
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

char	*mini_strcat(char *dest, const char *src)
{
	char	*p_dest;

	p_dest = dest;
	while (*p_dest != '\0')
		p_dest++;
	while (*src != '\0')
	{
		*p_dest = *src;
		p_dest++;
		src++;
	}
	*p_dest = '\0';
	return (dest);
}

int	is_operator(char *str)
{
	char	*operators[5];
	size_t	num_operators;
	size_t	i;

	if (!str)
		return (0);
	operators[0] = ">";
	operators[1] = "<";
	operators[2] = ">>";
	operators[3] = "<<";
	operators[4] = "|";
	num_operators = sizeof(operators) / sizeof(operators[0]);
	i = 0;
	while (i < num_operators)
	{
		if (ft_strcmp(str, operators[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	mini_isspace(int c)
{
	if (c != '\0' && c <= 32)
		return (1);
	return (0);
}
