/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:01:26 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/23 14:06:26 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	inorder_traversal(t_node *root)
{
	if (root == NULL)
		return ;
	inorder_traversal(root->left);
	if (root->operator)
		printf("Operator:%s$\n", (root->operator));
	else
		printf("Command:%s$\n", root->command);
	inorder_traversal(root->right);
}

int	is_only_spaces(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] > 32)
			return (0);
	}
	return (1);
}

void	keep_on_shell(t_cmd *cmd)
{
	add_history(cmd->line);
	cmd->line = expanding(cmd->line, cmd);
	cmd->root = init_shell(cmd->line);
	inorder_traversal(cmd->root);
	if (cmd->root && cmd->line)
	{
		init_args_next(cmd);
		if (cmd->size == 1)
			mini_built_in(cmd, &(cmd->g_env_list));
		else
			exec(cmd);
		free_tree(cmd->root);
	}
	free_ms(cmd);
}

int	main(void)
{
	t_cmd	*cmd;

	cmd = init_before_init();
	while (1)
	{
		init_args_ofen(cmd);
		cmd->line = readline("minishell$> ");
		if (!cmd->line)
			return (0 * printf("exit\n") + 1);
		if (!is_only_spaces(cmd->line))
		{
			if (checks_str(cmd))
				continue ;
			keep_on_shell(cmd);
		}
		free(cmd->line);
	}
	return (0);
}
