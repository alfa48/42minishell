/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:03:32 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/24 10:43:22 by manandre         ###   ########.fr       */
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
	if (ft_strcmp(first_word, "echo") == 0)
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

static int	is_spacial_command(char *str)
{
	if (is_first_word_echo(str) || is_first_word_echo(&str[1]))
		return (1);
	if (mini_strstr(str, "<") || mini_strstr(str, ">") || mini_strstr(str, "|"))
	{
		if (is_within_quotes(str, "<") || is_within_quotes(str, ">")
			|| is_within_quotes(str, "|"))
			return (1);
	}
	if (is_entirely_within_quotes(str))
		return (1);
	return (0);
}

char	*mini_epur_str(char *str)
{
	char	out[42000];
	int		i;

	if (is_spacial_command(str))
		return (ft_strdup(str));
	str = process_cmd(str);
	i = 0;
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
