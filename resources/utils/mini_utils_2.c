/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:03:32 by fjilaias          #+#    #+#             */
/*   Updated: 2024/12/02 15:14:24 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_node	*add_node(t_node *root, t_node *new, int side)
{
	if (root)
	{
		if (side == 0)
			root->left = new;
		else
			root->right = new;
	}
	return (new);
}

t_node	*create_node(char *operator, char *command, int index)
{
	t_node *new;

	new = malloc(sizeof(t_node));
	if (new)
	{
		new->index = index;
		new->operator = operator;
		new->command = command;
		new->left = NULL;
		new->right  =NULL;
	}
	return (new);
}

char	*mini_epur_str(char *str)
{
	char	out[42000];
	int	i;

	i = 0;
	while (*str == ' ' || *str == '\t')
		str ++;
	while (*str)
	{
		out[i++] = *str;
		if (*str++ == ' ')
		{
			while (*str == ' ' || *str == '\t')
				str ++;
			if (!*str)
				i --;
		}
	}
	out[i] = '\0';
	if (i > 0)
		return (ft_strdup(out));
	else
		return (NULL);
}
