/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_envexit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:17:02 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/30 20:41:10 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	mini_exit(t_cmd *cmd)
{
	char	*str;
	int		flag;

	flag = 0;
	if (cmd->arg[1])
	{
		str = cmd->arg[1];
		while (*str)
		{
			if (ft_isalpha(*str))
			{
				printf("minishell: exit: %s: numeric argument required\n",
					cmd->arg[1]);
				flag = 1;
				break ;
			}
			str++;
		}
		if (!flag && cmd->arg[2])
			printf("minishell: exit: too many arguments\n");
	}
	free_all(cmd);
	exit(0);
}

void	mini_env(t_env_var *g_env_list)
{
	t_env_var	*current;

	current = g_env_list;
	while (current)
	{
		printf("%s=%s\n", current->name, current->value);
		current = current->next;
	}
}

void	mini_pwd(void)
{
	char	cwd[42000];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("pwd");
}

void	execute_in_child(char *path, char **args)
{
	if (execve(path, args, NULL) == -1)
	{
		free(path);
		exit(EXIT_FAILURE);
	}
	free(path);
}
