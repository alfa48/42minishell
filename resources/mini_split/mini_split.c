/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:18:16 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/30 20:20:18 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_inside_quotes(const char *str, size_t pos)
{
	bool	in_quotes;
	size_t	i;

	in_quotes = false;
	i = 0;
	while (i < pos && str[i])
	{
		if (str[i] == '"' || str[i] == 39)
			in_quotes = !in_quotes;
		i++;
	}
	return (in_quotes);
}

static size_t	set_array_size(const char *str, char delimiter)
{
	size_t	size;
	size_t	pos;

	size = 0;
	pos = 0;
	while (str[pos])
	{
		if ((pos == 0 || str[pos - 1] == delimiter) && str[pos] != delimiter
			&& !is_inside_quotes(str, pos))
			size++;
		pos++;
	}
	return (size);
}

static size_t	word_position(const char *str, char delimiter)
{
	size_t	pos;

	pos = 0;
	while (str[pos])
	{
		if (str[pos] == '"' && !is_inside_quotes(str, pos))
		{
			pos++;
			while (str[pos] && !(str[pos] == '"' && !is_inside_quotes(str,
						pos)))
				pos++;
		}
		if (str[pos] == delimiter && !is_inside_quotes(str, pos))
			break ;
		if (str[pos])
			pos++;
	}
	return (pos);
}

char	**mini_split(char const *s, char c)
{
	char	**final_array;
	size_t	limit;
	size_t	array_size;
	size_t	counter;

	if (!s)
		return (NULL);
	array_size = set_array_size(s, c);
	final_array = malloc(sizeof(char *) * (array_size + 1));
	if (!final_array)
		return (NULL);
	counter = 0;
	while (*s && counter < array_size)
	{
		while (*s == c && is_inside_quotes(s, 0) == false)
			s++;
		limit = word_position(s, c);
		final_array[counter] = ft_substr(s, 0, limit);
		s += limit;
		counter++;
	}
	final_array[counter] = NULL;
	return (final_array);
}
