/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils_4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:13:10 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/29 16:19:36 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_var(char *name, t_env_var *env_list)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->name, name) == 0)
		{
			return (env_list->value);
		}
		env_list = env_list->next;
	}
	return (NULL);
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
		i++;
	}
	return (in_single_quote || in_double_quote);
}

int	is_entirely_within_quotes(char *str)
{
	if ((str[0] == '\'' && str[ft_strlen(str) - 1] == '\'') || (str[0] == '\"'
			&& str[ft_strlen(str) - 1] == '\"'))
		return (1);
	return (0);
}

char	*ft_strncpy(char *dst, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dst[i] = '\0';
		i++;
	}
	return (dst);
}
