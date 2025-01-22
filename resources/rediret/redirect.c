/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:57:35 by manandre          #+#    #+#             */
/*   Updated: 2025/01/22 10:26:01 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_redirect_char(const char *str, int *len)
{
	if (!str || !*str)
		return (0);
	if (str[0] == '<' || str[0] == '>')
	{
		*len = (str[1] == str[0]) ? 2 : 1;
		return (1);
	}
	return (0);
}

static char	*extract_token(const char **str)
{
	int			redirect_len;
	const char	*start;
	const char	*end;
	char		*token;

	if (!str || !*str || !**str)
		return (NULL);
	// Skip whitespace
	while (**str && (**str == ' ' || **str == '\t'))
		(*str)++;
	if (!**str)
		return (NULL);
	start = *str;
	redirect_len = 0;
	// Check if current position is a redirect character
	if (is_redirect_char(start, &redirect_len))
	{
		token = strndup(start, redirect_len);
		*str += redirect_len;
		return (token);
	}
	// If not redirect, read until we find a redirect
	while (**str && !is_redirect_char(*str, &redirect_len))
		(*str)++;
	if (start == *str)
		return (NULL);
	// Remove trailing spaces
	end = *str;
	while (end > start && (*(end - 1) == ' ' || *(end - 1) == '\t'))
		end--;
	return (ft_strndup(start, end - start));
}

char	**ft_split_redirect(const char *str)
{
	int			i;
	int			token_count;
	const char	*counter;
	const char	*parser;
	char		*temp_token;
	char		**result;

	if (!str)
		return (NULL);
	// First pass: count tokens
	counter = str;
	token_count = 0;
	while ((temp_token = extract_token(&counter)))
	{
		token_count++;
		free(temp_token);
	}
	// Allocate array (+2 for NULL at start and end)
	result = calloc(token_count + 2, sizeof(char *));
	if (!result)
		return (NULL);
	// Second pass: fill array
	parser = str;
	i = 1; // Start at 1 to leave NULL at index 0
	while ((temp_token = extract_token(&parser)) && i <= token_count)
		result[i++] = temp_token;
	result[i] = NULL; // Ensure NULL termination
	return (result);
}

void	exec_command_redirect(int pos, t_cmd *cmd)
{
	int	i;

	(void)cmd;
	(void)pos;
	// TODO: Implement pipeline execution here.
	i = 1;
	while (cmd->array_redirect[i])
	{
		execute_redirect_(i, cmd);
		i++;
	}
}
