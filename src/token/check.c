/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 14:35:09 by manandre          #+#    #+#             */
/*   Updated: 2024/11/07 15:01:29 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*check_redirection(char c, t_cmd_line *c_line);
t_token *check_condicitional(t_cmd_line *c_line);
t_token *check_keyword(t_cmd_line *cmd);
t_token	*check_special_characters(t_cmd_line *cmd);
t_token	*check_end(t_cmd_line *cmd);

t_token *check_condicitional(t_cmd_line *cmd)
{
	char tmp;

	tmp = gnc_buffer(cmd);
	if (tmp == '&')
	{
		tmp = gnc_buffer(cmd);
		if (tmp == '&')
		{
			return (create_token(CONDITIONAL_AND, "&&"));
		}
		gpc_buffer(cmd);
		return (create_token(BACKGROUND_COMMAND, "&"));
	}
	gpc_buffer(cmd);
	return (NULL);
}

t_token *check_keyword(t_cmd_line *cmd)
{
    char	lexema[100];
    char	tmp;
    size_t	len;

    tmp = gnc_buffer(cmd);
    len = 0;
    while (tmp != '\0')
    {
        if (ft_isalpha(tmp)) 
        {
            lexema[len++] = tmp;
            tmp = gnc_buffer(cmd);
        }
        else
        {
		gpc_buffer(cmd);
            break;
        }
    }
    lexema[len] = '\0';
    //printf("lexema : %s\n", cmd->lexema);
    if (len > 0)
	    return (create_token(EXTERNAL_COMMAND, lexema));
    return (NULL);
 }

t_token	*check_special_characters(t_cmd_line *cmd)
{
	char	tmp;
	
	tmp = gnc_buffer(cmd);
	if (tmp == '\\')
		return (create_token(ESCAPE, "\\"));
	else if (tmp == ';')
		return (create_token(COMMAND_SEQUENCE, ";"));
	else if (tmp == '*')
		return (create_token(WILDCARD, "*"));
	gpc_buffer(cmd);
	return (NULL);
}

t_token	*check_end(t_cmd_line *cmd)
{
	char	c;

	c = gnc_buffer(cmd);
	if (c == '\0')
		return (create_token(END, "END"));
	gpc_buffer(cmd);
	return (NULL);
}
