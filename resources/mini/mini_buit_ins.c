/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_buit_ins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 11:12:25 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/21 11:12:30 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	mini_echo(char *arg)
{
	int		new_line;
	char	*init;

	new_line = 0;
	arg = process_cmd(arg);
	init = get_word(arg, &new_line);
	if (!init)
		return ;
	else
	{
		while (*init && (*init <= 32))
			init++;
		if (init)
			write(1, init, ft_strlen(init));
	}
	if (new_line)
		write(1, "\n", 1);
}

void	mini_cd(char *path, t_env_var *g_env_list)
{
	char	cwd[PATH_MAX];

	if (chdir(path) != 0)
	{
		perror("cd");
		return ;
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		set_or_add_env_var(ft_strjoin("PWD=", cwd), &g_env_list);
	else
		perror("getcwd");
}

void	mini_export(char **args, t_env_var **g_env_list)
{
	int	i;

	if (!args[1])
	{
		only_expor_cmd(*g_env_list);
		return ;
	}
	i = 1;
	set_or_add_env_var(process_cmd(args[i]), g_env_list);
}

void	mini_unset(char **args, t_env_var **g_env_list)
{
	t_env_var	*current;
	t_env_var	*tmp;

	if (args[0] == NULL || args[1] == NULL)
		return ;
	current = *g_env_list;
	if (strcmp(args[1], current->name) == 0)
	{
		(*g_env_list) = current->next;
		current = NULL;
		return ;
	}
	while (current->next)
	{
		if (strcmp(args[1], current->next->name) == 0)
		{
			tmp = current->next;
			current->next = current->next->next;
			free(tmp);
			return ;
		}
		current = current->next;
	}
}
