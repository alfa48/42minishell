/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils_7.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 10:50:04 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/27 11:21:38 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_word(const char *start, const char *end)
{
	size_t	len;
	char	*word;

	len = end - start;
	word = (char *)malloc(len + 1);
	if (!word)
		return (NULL);
	strncpy(word, start, len);
	word[len] = '\0';
	return (word);
}

const char	*skip_spaces(const char *line)
{
	while (*line && mini_isspace((unsigned char)*line))
		line++;
	return (line);
}

const char	*find_quote_end(const char *line, char quote_char)
{
	while (*line && *line != quote_char)
		line++;
	return (line);
}

char	*get_first_word(const char *line)
{
	const char	*start;
	const char	*end;
	char		quote_char;

	quote_char = '\0';
	line = skip_spaces(line);
	if (*line == '\'' || *line == '\"')
	{
		quote_char = *line++;
		start = line;
		end = find_quote_end(line, quote_char);
		if (*end == quote_char)
			return (extract_word(start, end));
	}
	else
	{
		start = line;
		while (*line && !mini_isspace((unsigned char)*line))
			line++;
		end = line;
	}
	return (extract_word(start, end));
}
