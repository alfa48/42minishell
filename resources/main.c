/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:01:26 by fjilaias          #+#    #+#             */
/*   Updated: 2024/12/13 12:11:03 by fjilaias         ###   ########.fr       */
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
		printf("Command:%s$\n", (root->command));
	inorder_traversal(root->right);
}

int	main(void)
{
	t_cmd	*cmd;
	extern char	**environ;

	cmd = malloc(sizeof(t_cmd));
	cmd->g_env_list = NULL;
	initialize_env_list(&(cmd->g_env_list), environ);
	init_args(cmd);
	if (!cmd->g_env_list)
		return (0 * printf("Error in create env list!\n"));
	while (1)
	{
		cmd->line = readline("minishell$> ");
		if (cmd->line || cmd->line[0] != '\0')
		{

			cmd->root = init_shell(cmd->line);
			if (cmd->root)
			{
				init_args_next(cmd);
				if (!(cmd->line = expanding(cmd->line, cmd->g_env_list)))
					return (0);
				if (cmd->size == 1)
					mini_built_in(cmd, &(cmd->g_env_list));
				else
					exec(cmd, environ);
				free_tree(cmd->root);
			}
			cmd->index = 0;
			cmd->g_env_list->counter_exp = 0;
			free_ms(cmd);
			free(cmd->line);
		}
	}
	return (0);
}
