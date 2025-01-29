/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:27:16 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/29 09:55:50 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	compare_env_vars(const void *a, const void *b)
{
	t_env_var	*env_a;
	t_env_var	*env_b;

	env_a = *(t_env_var **)a;
	env_b = *(t_env_var **)b;
	return (ft_strcmp(env_a->name, env_b->name));
}

t_env_var	*clone_env_list(t_env_var *original)
{
	t_env_var	*new_list;
	t_env_var	*new_node;
	t_env_var	*current;

	new_list = NULL;
	while (original)
	{
		new_node = malloc(sizeof(t_env_var));
		if (!new_node)
			return (NULL);
		new_node->name = ft_strdup(original->name);
		new_node->value = ft_strdup(original->value);
		new_node->next = NULL;
		if (!new_list)
			new_list = new_node;
		else
		{
			current = new_list;
			while (current->next)
				current = current->next;
			current->next = new_node;
		}
		original = original->next;
	}
	return (new_list);
}

void	sort_env_list(t_env_var *head)
{
	t_env_var	*i;
	t_env_var	*j;
	char		*temp_name;
	char		*temp_value;

	i = head;
	while (i != NULL)
	{
		j = i->next;
		while (j != NULL)
		{
			if (ft_strcmp(i->name, j->name) > 0)
			{
				// Troca os valores
				temp_name = i->name;
				temp_value = i->value;
				i->name = j->name;
				i->value = j->value;
				j->name = temp_name;
				j->value = temp_value;
			}
			j = j->next;
		}
		i = i->next;
	}
}

void	only_expor_cmd(t_env_var *g_env_list)
{
	t_env_var	*cloned_list;
	t_env_var	*current;

	if (!g_env_list)
		return ;
	cloned_list = clone_env_list(g_env_list);
	if (!cloned_list)
		return ;
	sort_env_list(cloned_list);
	current = cloned_list;
	while (current)
	{
		if (current->value)
			printf("declare -x %s=\"%s\"\n", current->name, current->value);
		else
			printf("declare -x %s\n", current->name);
		current = current->next;
	}
	free_env_list(&(cloned_list));
}
