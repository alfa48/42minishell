/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 14:36:32 by manandre          #+#    #+#             */
/*   Updated: 2024/11/15 09:08:49 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(int type, char *lexema);
t_token	*next_token(t_cmd_line *c_line);

t_token	*create_token(int type, char *lexema)
{
	t_token *token;
	
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->lexema = ft_strdup(lexema);
	return (token);
}

t_token	*next_token(t_cmd_line *cmd)
{
	t_token	*token;
	
	token = NULL;
	check_space(cmd);
	ft_enter(cmd);

	t_token* (*funArry[])(t_cmd_line *) = {check_end, check_condicitional,
	check_pipe_or, check_ex_cmd, check_relative_path, check_env,
	check_special_characters, check_high_commas, check_litle_high_commas,
	check_redirectors, check_hifem, check_absolutely_path};

	size_t	i = 0;
	while (i < (sizeof(funArry) / sizeof(funArry[0])))
	{
		token = (funArry[i](cmd));
		if (token == NULL)
			ft_clear(cmd);
		else
		{
			ft_enter(cmd);
			return (token);
		}
		i ++;
	}
	return (NULL);
}