/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_execv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:56:05 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/30 19:29:57 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	mini_built_in(t_cmd *cmd, t_env_var **g_env_list)
{
	cmd->status_cmd = 0;
	cmd->arg = mini_split(cmd->root->command, ' ');
	if (!cmd)
		return ;
	if (ft_strcmp("cd", cmd->arg[0]) == 0)
		mini_cd(cmd->arg[1], *g_env_list);
	else if (ft_strcmp("pwd", cmd->arg[0]) == 0)
		mini_pwd();
	else if (ft_strcmp("exit", cmd->arg[0]) == 0)
		mini_exit(cmd);
	else if (ft_strcmp("export", cmd->arg[0]) == 0)
		mini_export(cmd->arg, g_env_list);
	else if (ft_strcmp(cmd->arg[0], "env") == 0)
		mini_env(*g_env_list);
	else if (ft_strcmp(cmd->arg[0], "unset") == 0)
		mini_unset(cmd->arg, g_env_list);
	else if (ft_strcmp("echo", cmd->arg[0]) == 0)
		mini_echo(cmd->line, cmd->line1);
	else
	{
		fork_exec_cmd(cmd, cmd->root);
		wait_forks(cmd);
	}
}

void	list_env_vars(t_env_var *g_env_list)
{
	t_env_var	*current;

	current = g_env_list;
	while (current)
	{
		printf("%s=%s\n", current->name, current->value);
		current = current->next;
	}
}

int	env_list_size(t_env_var *g_env_list)
{
	int			count;
	t_env_var	*current;

	count = 0;
	current = g_env_list;
	if (!current)
		return (-1);
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return (count);
}
