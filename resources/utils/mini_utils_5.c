/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils_5.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 10:46:09 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/21 10:50:26 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_quotes_balance(const char *arg)
{
	bool	in_single_quote;
	bool	in_double_quote;

	in_single_quote = false;
	in_double_quote = false;
	while (*arg)
	{
		if (*arg == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*arg == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		arg++;
	}
	if (!in_single_quote && !in_double_quote)
		return (true);
	else
	{
		printf("Error: unclosed quotes\n");
		return (false);
	}
}

void	free_tree(t_node *root)
{
	if (root == NULL)
		return ;
	free_tree(root->left);
	if (root->command)
		free(root->command);
	free_tree(root->right);
	free(root);
}

int	is_echo_printable(char c)
{
	if (is_special_char(c))
		return (1);
	if (ft_isprint(c))
		return (1);
	return (0);
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
				return (tmp->value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}