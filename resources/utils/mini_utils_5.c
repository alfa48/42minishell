/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils_5.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 10:46:09 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/30 23:50:16 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_quotes_balance(const char *arg)
{
	char	*tmp;
	bool	in_single_quote;
	bool	in_double_quote;

	in_single_quote = false;
	in_double_quote = false;
	tmp = (char *)arg;
	while (*tmp)
	{
		if (*tmp == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*tmp == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		tmp++;
	}
	if (!in_single_quote && !in_double_quote)
		return (true);
	else
	{
		printf("Error: unclosed quotes\n");
		return (false);
	}
}

bool	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	*ft_findenv(char *s, t_env_var *g_env_list)
{
	size_t		len;
	t_env_var	*tmp;

	tmp = g_env_list;
	while (tmp)
	{
		len = ft_strlen(tmp->name);
		if (ft_strncmp(s, tmp->name, len) == 0)
		{
			if (!is_valid_var_char(s[len]))
				return (ft_strdup(tmp->value));
		}
		tmp = tmp->next;
	}
	return (NULL);
}

int	is_valid_env_var_name(const char *str)
{
	if (!str || !*str)
		return (0);
	if ((!ft_isalpha(*str) && *str != '_') || ft_strlen(str) > 255)
	{
		ft_putstr_fd("export: invalid env name\n", 2);
		return (0);
	}
	while (*++str)
	{
		if (!ft_isalnum(*str) && *str != '_')
		{
			ft_putstr_fd("export: invalid env name\n", 2);
			return (0);
		}
	}
	return (1);
}
