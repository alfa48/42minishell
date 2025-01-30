/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:03:32 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/30 21:04:18 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_first_word_echo(char *command)
{
	char	first_word[256];
	int		i;

	i = 0;
	while (mini_isspace(*command))
		command++;
	while (*command && !mini_isspace(*command) && i < 255)
		first_word[i++] = *command++;
	first_word[i] = '\0';
	if (ft_strcmp(first_word, "echo") == 0)
		return (1);
	return (0);
}

static char	*return_epur(int *i, char *str, char *unsed)
{
	free(unsed);
	str[*i] = '\0';
	if ((*i) > 0)
		return (str);
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

static char	*auxi_epur(char *str)
{
	return (ft_strdup(str));
}

char	*mini_epur_str(char *str)
{
	char	*out;
	int		i;
	char	*tmp;
	char	*tmp1;

	if (is_spacial_command(str))
		return (auxi_epur(str));
	tmp = process_cmd(str);
	tmp1 = tmp;
	i = 0;
	out = malloc(sizeof(char) * ft_strlen(tmp) + 1);
	while (*tmp1 == ' ' || *tmp1 == '\t')
		tmp1++;
	while (*tmp1)
	{
		out[i++] = *tmp1;
		if (*tmp1++ == ' ')
		{
			while (*tmp1 == ' ' || *tmp1 == '\t')
				tmp1++;
			if (!*tmp1)
				i--;
		}
	}
	return (return_epur(&i, out, tmp));
}
