/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:24:48 by manandre          #+#    #+#             */
/*   Updated: 2025/01/27 11:02:41 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_args_size(char **cmd_args)
{
	int	size;

	size = 0;
	while (cmd_args[size] != NULL)
		size++;
	return (size);
}

char	**get_args(char *cmd)
{
	char	**cmd_args;
	char	**execve_args;
	int		i;

	i = 0;
	if (!is_entirely_within_quotes(cmd))
	{
		cmd_args = ft_split(cmd, ' ');
		execve_args = malloc((get_args_size(cmd_args) + 1) * sizeof(char *));
		execve_args[0] = cmd_args[0];
		while (++i < get_args_size(cmd_args))
			execve_args[i] = cmd_args[i];
		execve_args[i] = NULL;
	}
	else
	{
		execve_args = malloc(2 * sizeof(char *));
		execve_args[0] = process_cmd(cmd);
		execve_args[1] = NULL;
	}
	return (execve_args);
}

int	is_redirect(char *str)
{
	char	*operators[4];
	size_t	num_operators;
	size_t	i;

	if (!str)
		return (0);
	operators[0] = ">";
	operators[1] = "<";
	operators[2] = ">>";
	operators[3] = "<<";
	num_operators = sizeof(operators) / sizeof(operators[0]);
	i = 0;
	while (i < num_operators)
	{
		if (ft_strcmp(str, operators[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

// Função auxiliar para verificar precedência de redirecionamentos
int	get_redirect_priority(char *type)
{
	if (ft_strcmp(type, "<") == 0)
		return (1);
	if (ft_strcmp(type, ">") == 0)
		return (2);
	if (ft_strcmp(type, ">>") == 0)
		return (2);
	return (0);
}

// Função para ordenar redirecionamentos por precedência
void	sort_redirects(t_redirect **redirects)
{
	int			i;
	int			j;
	int			one;
	t_redirect	*temp;

	if (!redirects)
		return ;
	i = 0;
	while (redirects[i] && redirects[i + 1])
	{
		j = 0;
		while (redirects[j] && redirects[j + 1])
		{
			one = get_redirect_priority(redirects[j + 1]->type);
			if (get_redirect_priority(redirects[j]->type) > one)
			{
				temp = redirects[j];
				redirects[j] = redirects[j + 1];
				redirects[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}
