/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_before.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 16:21:00 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/27 11:29:15 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*init_before_init(void)
{
	extern char	**environ;
	t_cmd		*cmd;

	cmd = malloc(sizeof(t_cmd));
	handle_signals();
	init_args(cmd, environ);
	return (cmd);
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
	t_node	*new;

	new = malloc(sizeof(t_node));
	if (new)
	{
		new->index = index;
		new->operator = operator;
		new->command = command;
		new->left = NULL;
		new->right = NULL;
	}
	return (new);
}
