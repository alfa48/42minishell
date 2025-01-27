/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils_6.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 14:47:11 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/24 10:23:50 by manandre         ###   ########.fr       */
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

void	*ft_realloc(void *ptr, size_t new_size)
{
	void	*new_ptr;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (ptr == NULL)
		return (malloc(new_size));
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, new_size);
	free(ptr);
	return (new_ptr);
}
