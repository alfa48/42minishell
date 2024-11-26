/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_aux.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 16:08:57 by fjilaias          #+#    #+#             */
/*   Updated: 2024/11/15 09:09:24 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*check_redirectors(t_cmd_line *cmd)
{
	char tmp;

	tmp = gnc_buffer(cmd);
	if (tmp == '>')
	{
		tmp = gnc_buffer(cmd);
		if (tmp == '>')
			return (create_token(REDIRECTION, ">>"));
		gpc_buffer(cmd);
		return (create_token(REDIRECTION, ">"));
	}
	else if (tmp == '<')
	{
		tmp = gnc_buffer(cmd);
		if (tmp == '<')
			return (create_token(REDIRECTION, "<<"));
		gpc_buffer(cmd);
		return (create_token(REDIRECTION, "<"));
	}
	gpc_buffer(cmd);
	return (NULL);
}

static char process_path_segment(t_cmd_line *cmd, char *tmp, int *i)
{
	while (*tmp && ft_isalpha(*tmp))
	{
		*tmp = gnc_buffer(cmd);
		(*i) ++;
	}
	if (*tmp == '\0')
	{
		*tmp = gnc_buffer(cmd);
		if (ft_isalpha(*tmp))
			return (process_path_segment(cmd, tmp, i));
		else
			gpc_buffer(cmd);
	}
	return (*tmp);
}

t_token	*check_relative_path(t_cmd_line *cmd)
{
	int	i;
	char	tmp;

	i = 0;
	tmp = gnc_buffer(cmd);
	if (tmp)
		tmp = process_path_segment(cmd, &tmp, &i);
	if (i > 0)
	{
		gpc_buffer(cmd);
		return (create_token(PATH, cmd->lexema));
	}
	return (NULL); 
}

t_token	*check_absolutely_path(t_cmd_line *cmd)
{
	char	tmp;

	tmp = gnc_buffer(cmd);
	if (tmp == '/')
	{
		while (tmp)
			tmp = gnc_buffer(cmd);
		if (tmp == '\0')
			return (create_token(PATH, cmd->lexema));
	}
	return (NULL); 
}

t_token	*check_env(t_cmd_line *cmd)
{
	char	tmp;
	int		is_env;

	is_env = 0;
	tmp = gnc_buffer(cmd);
	if (tmp)
	{
		while (tmp && tmp != ' ')
		{
			if (tmp == '=' && (gnc_buffer(cmd)))
				is_env = 1;
			tmp = gnc_buffer(cmd);
		}
		if (tmp == '\0' && is_env)
			return (create_token(PATH, cmd->lexema));
	}
	return (NULL);
}

// Função auxiliar para obter sequência alfabética


/*t_token	*check_ex_cmd(t_cmd_line *cmd)
{
	char	tmp;
	int	i;
	
	i = 0;
	tmp = gnc_buffer(cmd); 
	tmp = get_alpha(cmd, tmp, &i);
	if (i > 0)
	{
		if (tmp != '\0' && tmp != '/')
			gpc_buffer(cmd);
		else if (tmp == '/')
		{
			tmp = gnc_buffer(cmd);
			if (ft_isalpha(tmp))
			{
				i = 0;
				get_alpha(cmd, tmp, &i);
			}
			else
				gpc_buffer(cmd);
			
			if (tmp != '\0')
				gpc_buffer(cmd);			
		}
			
		return (create_token(EXTERNAL_COMMAND, cmd->lexema));
	}
	gpc_buffer(cmd);
	return (NULL);
}

// src/ext
static char get_nxt(t_cmd_line *cmd, char tmp, int *i)
{
	while (tmp && ft_isalpha(tmp))
	{
		tmp = gnc_buffer(cmd);
		(*i)++;
	}
	return (tmp);
}*/

t_token	*check_ex_cmd(t_cmd_line *cmd)
{
	char	tmp;
	int	i;
	
	i = 0;
	tmp = gnc_buffer(cmd); 
	while (tmp && ft_isalpha(tmp))
	{
		tmp = gnc_buffer(cmd);
		i++;
	}
	if (i > 0)
	{
		if (tmp != '\0')
			gpc_buffer(cmd);
		return (create_token(EXTERNAL_COMMAND, cmd->lexema));
	}
	gpc_buffer(cmd);
	return (NULL);
}

t_token *check_high_commas(t_cmd_line *cmd)
{
	char	tmp;

	if (gnc_buffer(cmd) != 34)
		return (NULL);
	tmp = gnc_buffer(cmd);
	while (tmp != 34 && tmp)
		tmp = gnc_buffer(cmd);
	if (tmp == '\0')
		return (create_token(QUOTE, "quote"));
	else if (tmp == 34)
		return (create_token(STRING, cmd->lexema));
	return (NULL);
}

t_token *check_litle_high_commas(t_cmd_line *cmd)
{
	char	tmp;

	if (gnc_buffer(cmd) != 39)
		return (NULL);
	tmp = gnc_buffer(cmd);
	while (tmp != 39 && tmp)
		tmp = gnc_buffer(cmd);
	if (tmp == '\0')
		return (create_token(QUOTE, "quote"));
	else if (tmp == 39)
		return (create_token(STRING, cmd->lexema));
	return (NULL);
}

t_token	*check_hifem(t_cmd_line *cmd)
{
	char	tmp;
	int	i;

	i = 0;
	tmp = gnc_buffer(cmd);
	if (tmp == '-')
	{
		tmp = gnc_buffer(cmd);
		while (tmp && ft_isalpha(tmp))
		{
			tmp = gnc_buffer(cmd);
			i ++;
		}
		if (i > 0)
		{
			if (tmp != '\0')
				gpc_buffer(cmd);
			return (create_token(OPTION, cmd->lexema));
		}
	}
	return (NULL);
}