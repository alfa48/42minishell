/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_buit_ins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 11:12:25 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/28 16:30:26 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	mini_echo(char *arg, char *siline)
{
	int		new_line;
	char	*init;
	char	*echo;

	new_line = 0;
	arg = process_cmd(arg);
	init = get_word(arg, &new_line, siline);
	if (!init)
		return ;
	else
	{
		while (*init && (*init <= 32))
			init++;
		if (init && new_line == 1)
		{
			echo = ft_strjoin(init, "\n");
			printf("%s", echo);
			free(echo);
		}
		else if (init && new_line == 0)
			printf("%s", init);
	}
}

void	mini_cd(char *path, t_env_var *g_env_list)
{
	char	cwd[PATH_MAX];
	char	*tmp;


	if (chdir(path) != 0)
	{
		perror("cd");
		return ;
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		tmp = ft_strjoin("PWD=", cwd);
		set_or_add_env_var(tmp, &g_env_list);
		free(tmp);

	}
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
	if (ft_strcmp(args[1], current->name) == 0)
	{
		(*g_env_list) = current->next;
		current = NULL;
		return ;
	}
	while (current->next)
	{
		if (ft_strcmp(args[1], current->next->name) == 0)
		{
			tmp = current->next;
			current->next = current->next->next;
			free(tmp);
			return ;
		}
		current = current->next;
	}
}
