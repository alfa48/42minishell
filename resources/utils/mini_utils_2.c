/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:03:32 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/21 13:05:39 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_first_word_echo(char *command)
{
	char	first_word[256];
	int		i;

	i = 0;
	while (isspace(*command))
		command++;
	while (*command && !isspace(*command) && i < 255)
		first_word[i++] = *command++;
	first_word[i] = '\0';
	if (strcmp(first_word, "echo") == 0)
		return (1);
	return (0);
}

static char	*return_epur(int *i, char *str)
{
	if ((*i) > 0)
		return (ft_strdup(str));
	else
		return (NULL);
}

char	*mini_epur_str(char *str)
{
	char	*out;
	int		i;

	if (is_first_word_echo(str) || is_first_word_echo(&str[1]))
		return (ft_strdup(str));
	if (is_entirely_within_quotes(str))
		return (ft_strdup(str));
	str = process_cmd(str);
	i = 0;
	out = malloc(sizeof(char) * ft_strlen(str) + 1);
	while (*str == ' ' || *str == '\t')
		str++;
	while (*str)
	{
		out[i++] = *str;
		if (*str++ == ' ')
		{
			while (*str == ' ' || *str == '\t')
				str++;
			if (!*str)
				i--;
		}
	}
	out[i] = '\0';
	return (return_epur(&i, out));
}

int	is_special_char(char c)
{
	int			i;
	const char	*special_chars;

	i = 0;
	special_chars = ", +-=/^@#%:~´.'";
	while (special_chars[i] != '\0')
	{
		if (c == special_chars[i])
			return (1);
		i++;
	}
	return (0);
}
