/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 20:35:26 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/30 21:26:17 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_lines(t_cmd *cmd)
{
	if (cmd->line)
		free(cmd->line);
	if (cmd->line1)
		free(cmd->line1);
}

void	free_one_iterator(t_cmd *cmd)
{
	free_lines(cmd);
	free_tree(cmd->root);
	free_arg(cmd);
	free_cmd_array(cmd);
}

int	free_unset(t_env_var *current)
{
	t_env_var	*tmp;

	if (!current)
		return (0);
	tmp = current->next;
	current->next = current->next->next;
	free(tmp->name);
	free(tmp->value);
	free(tmp);
	return (1);
}

// Função para liberar a estrutura de redirecionamentos
void	free_redirects(t_redirect **redirects)
{
	int	i;

	if (!redirects)
		return ;
	i = 0;
	while (redirects[i])
	{
		free(redirects[i]->type);
		free(redirects[i]->file);
		free(redirects[i]);
		i++;
	}
	free(redirects);
	redirects = NULL;
}

void	free_all(t_cmd *cmd)
{
	free_lines(cmd);
	free_cmd_array(cmd);
	free_arg(cmd);
	free_tree(cmd->root);
	free_env_list(&(cmd->g_env_list));
	free(cmd);
	cmd = NULL;
}
