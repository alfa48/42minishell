/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_inits.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 11:11:16 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/30 20:36:50 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_list(t_env_var **g_env_list)
{
	t_env_var	*temp;

	if (!g_env_list || !*g_env_list)
		return ;
	while (*g_env_list)
	{
		temp = *g_env_list;
		*g_env_list = (*g_env_list)->next;
		free(temp->name);
		free(temp->value);
		free(temp);
	}
}

int	write_exit(t_cmd *cmd)
{
	free_env_list(&(cmd->g_env_list));
	free_lines(cmd);
	free(cmd);
	return ((0 * write(1, "exit\n", 5)) + 1);
}

void	free_tree(t_node *root)
{
	if (!root)
		return ;
	free_tree(root->left);
	free_tree(root->right);
	if (root->command)
		free(root->command);
	free(root);
	root = NULL;
}

void	free_cmd_array(t_cmd *cmd)
{
	int	i;

	if (!cmd->array)
		return ;
	i = 0;
	while (cmd->array[++i])
		free(cmd->array[i]);
	free(cmd->array);
	cmd->array = NULL;
}

void	free_arg(t_cmd *cmd)
{
	int	i;

	if (!cmd->arg)
		return ;
	i = -1;
	while (cmd->arg[++i])
		free(cmd->arg[i]);
	free(cmd->arg);
	cmd->arg = NULL;
}
