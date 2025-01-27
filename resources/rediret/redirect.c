/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:57:35 by manandre          #+#    #+#             */
/*   Updated: 2025/01/24 10:10:29 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_there_redirect_char(const char *str, int *len)
{
	if (!str || !*str)
		return (0);
	if (str[0] == '<' || str[0] == '>')
	{
		if (str[1] == str[0])
			*len = 2;
		else
			*len = 1;
		return (1);
	}
	return (0);
}

static char	*extract_token(const char **str)
{
	int			redirect_len;
	const char	*start;
	const char	*end;

	if (!str || !*str || !**str)
		return (NULL);
	while (**str && (**str == ' ' || **str == '\t'))
		(*str)++;
	if (!**str)
		return (NULL);
	start = *str;
	if (is_there_redirect_char(start, &redirect_len))
		return (ft_strndup(start, redirect_len));
	while (**str && !is_there_redirect_char(*str, &redirect_len))
		(*str)++;
	end = *str;
	while (end > start && (*(end - 1) == ' ' || *(end - 1) == '\t'))
		end--;
	return (ft_strndup(start, end - start));
}

static int	count_tokens(const char *str)
{
	int			token_count;
	char		*temp_token;
	const char	*counter = str;

	token_count = 0;
	temp_token = extract_token(&counter);
	while (temp_token)
	{
		token_count++;
		free(temp_token);
	}
	return (token_count);
}

char	**ft_split_redirect(const char *str)
{
	const char	*parser;
	char		**result;
	char		*temp_token;
	int			token_count;
	int			i;

	if (!str)
		return (NULL);
	token_count = count_tokens(str);
	result = ft_calloc(token_count + 2, sizeof(char *));
	if (!result)
		return (NULL);
	parser = str;
	i = 1;
	temp_token = extract_token(&parser);
	while (temp_token && i <= token_count)
		result[i++] = temp_token;
	result[i] = NULL;
	return (result);
}

/*void	exec_command_redirect(int pos, t_cmd *cmd)
{
	int	i;

	(void)cmd;
	(void)pos;
	i = 1;
	while (cmd->array_redirect[i])
	{
		execute_redirect_(i, cmd);
		i++;
	}
}*/
