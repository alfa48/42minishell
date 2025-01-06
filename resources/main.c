/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:01:26 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/06 15:17:35 by fjilaias         ###   ########.fr       */
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

int is_only_spaces(char *str)
{
	while (*str)
	{
		if (*str != ' ' && *str != '\t')
			return (0);
		str++;
	}
	return (1);
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
	handle_signals();
	while (1)
	{
		cmd->line = readline("minishell$> ");
		if (cmd->line && !is_only_spaces(cmd->line))
		{
			add_history(cmd->line);
			cmd->root = init_shell(cmd->line);
			if (cmd->root)
			{
				cmd->line = expanding(cmd->line, cmd->g_env_list);
				init_args_next(cmd);
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
		else
		{
			if (cmd->line)
				free(cmd->line);
			else
				break;
		}
	}
	return (0);
}
