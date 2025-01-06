/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_vars.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 09:34:57 by manandre          #+#    #+#             */
/*   Updated: 2025/01/06 16:26:36 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	calc_tree_size(t_node *root)
{
    if (root == NULL)
        return 0;
    return (1 + calc_tree_size(root->left) + calc_tree_size(root->right));
}

void    init_args(t_cmd  *cmd)
{
	cmd->root = NULL;
	cmd->line = NULL;
	cmd->arg = NULL;
	cmd->nl = 0;
	cmd->ncmd = 0;
	cmd->val_only = NULL;
	cmd->g_env_list->counter_exp = 0;
	cmd->pid_count = 0;
}

void    init_args_next(t_cmd  *cmd)
{
    cmd->size = calc_tree_size(cmd->root);
    cmd->array = malloc(cmd->size * sizeof(char *));
    fill_array_cmd(cmd->root, cmd->array, &(cmd->index));
}

void	fill_array_cmd(t_node *root, char **array, int *index)
{
	if (root == NULL)
		return ;
	fill_array_cmd(root->left, array, index);
	if (root->command != NULL)
		array[*index] = root->command;
	else
		array[*index] = root->operator;
	(*index)++;
	root->index = *index;
	fill_array_cmd(root->right, array, index);
}

