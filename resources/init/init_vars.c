/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_vars.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 09:34:57 by manandre          #+#    #+#             */
/*   Updated: 2025/01/30 18:19:35 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	calc_tree_size(t_node *root)
{
	if (root == NULL)
		return (0);
	return (1 + calc_tree_size(root->left) + calc_tree_size(root->right));
}

void	init_args_ofen(t_cmd *cmd)
{
	cmd->root = NULL;
	cmd->line = NULL;
	cmd->arg = NULL;
	cmd->line1 = NULL;
	cmd->array = NULL;
	cmd->redirects = NULL;
	cmd->pid_count = 0;
	cmd->index = 0;
	g_signal_status = 0;
}

void	init_args(t_cmd *cmd, char **environ)
{
	cmd->envl = NULL;
	cmd->g_env_list = NULL;
	cmd->status_cmd = 0;
	cmd->envl = environ;
	initialize_env_list(&(cmd->g_env_list), cmd->envl);
	if (!cmd->g_env_list)
	{
		free(cmd);
		exit(1);
	}
	cmd->g_env_list->counter_exp = 0;
}

void	init_args_next(t_cmd *cmd)
{
	cmd->size = calc_tree_size(cmd->root);
	cmd->array = malloc((cmd->size + 2) * sizeof(char *));
	if (!cmd->array)
    	return;
	cmd->array[0] = NULL;
	cmd->index = 1;
	fill_array_cmd(cmd->root, cmd->array, &(cmd->index));
	cmd->array[cmd->index] = NULL;
}

void	fill_array_cmd(t_node *root, char **array, int *index)
{
	if (root == NULL)
		return ;
	fill_array_cmd(root->left, array, index);
	if (root->command != NULL)
		array[*index] = ft_strdup(root->command);
	else
		array[*index] = ft_strdup(root->operator);
	(*index)++;
	root->index = *index;
	fill_array_cmd(root->right, array, index);
}
