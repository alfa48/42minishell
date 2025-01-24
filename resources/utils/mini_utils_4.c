/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils_4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:13:10 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/24 15:44:49 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_var(const char *name, t_env_var *env_list)
{
	while (env_list)
	{
		if (strcmp(env_list->name, name) == 0)
		{
			return (env_list->value);
		}
		env_list = env_list->next;
	}
	return (NULL);
}

char	*get_first_word(const char *line)
{
	const char	*start;
	const char	*end;
	char		*word;
	size_t		len;

	// Avança até o primeiro caractere não-espaço
	while (*line && isspace((unsigned char)*line))
		line++;
	if (*line == '\0')
		return (NULL);

	// Encontra o final da palavra
	start = line;
	while (*line && !isspace((unsigned char)*line))
		line++;
	end = line;

	// Calcula o comprimento da palavra
	len = end - start;

	// Aloca memória para a nova string (mais 1 para o terminador nulo)
	word = (char *)malloc(len + 1);
	if (!word)
		return (NULL);

	// Copia a palavra para a nova string e adiciona o terminador nulo
	strncpy(word, start, len);
	word[len] = '\0';

	return (word);
}



void	cmd_not_found(char *str)
{
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": command not found\n", 2);
}

int	is_within_quotes(char *str, char *needed)
{
	int		in_single_quote;
	int		in_double_quote;
	int		i;
	char	*found;

	in_single_quote = 0;
	in_double_quote = 0;
	i = 0;
	if (!str || !needed || ft_strlen(needed) == 0)
		return (0);
	found = mini_strstr(str, needed);
	if (!found)
		return (0);
	while (&str[i] != found)
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		i ++;
	}
	return (in_single_quote || in_double_quote);
}

int	is_entirely_within_quotes(char *str)
{
	if ((str[0] == '\'' && str[strlen(str) - 1] == '\'') || (str[0] == '\"'
			&& str[strlen(str) - 1] == '\"'))
		return (1);
	return (0);
}
