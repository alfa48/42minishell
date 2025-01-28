/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_execv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:56:05 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/27 12:33:00 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	mini_built_in(t_cmd *cmd, t_env_var **g_env_list)
{
	cmd->status_cmd = 0;
	cmd->arg = ft_split(cmd->root->command, ' ');
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
		printf("%s\n", cmd->root->command);
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

static int	compare_env_vars(const void *a, const void *b)
{
	t_env_var	*var_a;
	t_env_var	*var_b;

	var_a = *(t_env_var **)a;
	var_b = *(t_env_var **)b;
	return (ft_strcmp(var_a->name, var_b->name));
}

void	only_expor_cmd(t_env_var *g_env_list)
{
	int			i;
	int			count;
	t_env_var	*current;
	t_env_var	**env_array;

	i = -1;
	count = env_list_size(g_env_list);
	env_array = malloc(count * sizeof(t_env_var *));
	if (!env_array)
	{
		perror("malloc");
		return ;
	}
	current = g_env_list;
	while (++i < count)
	{
		env_array[i] = current;
		current = current->next;
	}
	ft_qsort(env_array, count, sizeof(t_env_var *), compare_env_vars);
	i = -1;
	while (++i < count)
		printf("declare -x %s=\"%s\"\n", env_array[i]->name,
			env_array[i]->value);
	free(env_array);
}
