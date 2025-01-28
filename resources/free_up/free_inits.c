/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_inits.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:11:16 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/28 12:58:16 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_list(t_env_var *g_env_list)
{
	t_env_var	*head;
	t_env_var	*tmp;

	head = g_env_list;
	if (!head)
		return ;
	while (head->next)
	{
		tmp = head;
		head = head->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
	free(head);
}

int	write_exit(t_cmd *cmd)
{
	free_env_list(cmd->g_env_list);
	return ((0 * write(1, "exit\n", 5)) + 1);
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

void	free_ms(t_cmd *cmd)
{
	int i;

	i = -1;
	if (!cmd)
		return ;
	if (cmd->arg)
	{
		while (cmd->arg[++i])
			free(cmd->arg[i]);
		free(cmd->arg);
		cmd->arg = NULL;
	}
	if (cmd->array)
		free(cmd->array);
}
