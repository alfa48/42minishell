/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 09:07:53 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/27 10:19:05 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Função para extrair redirecionamentos de um comandos
t_redirect	**parse_redirects(char *cmd_str, t_cmd *cmd)
{
	int			redirect_count;
	t_redirect	**redirects;
	char		**tokens;

	cmd->i = 0;
	redirects = malloc(sizeof(t_redirect *) * MAX_REDIRECTS);
	tokens = ft_split(cmd_str, ' ');
	redirect_count = 0;
	while (tokens[cmd->i] && redirect_count < MAX_REDIRECTS)
	{
		if (is_redirect(tokens[cmd->i]))
		{
			redirects[redirect_count] = malloc(sizeof(t_redirect));
			redirects[redirect_count]->type = ft_strdup(tokens[cmd->i]);
			redirects[redirect_count]->file = ft_strdup(tokens[cmd->i + 1]);
			redirects[redirect_count]->fd = -1;
			redirect_count++;
			cmd->i += 2;
		}
		else
			cmd->i++;
	}
	redirects[redirect_count] = NULL;
	free_array(tokens);
	return (redirects);
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
}
