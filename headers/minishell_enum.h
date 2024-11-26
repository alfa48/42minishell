/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_enum.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 14:08:07 by manandre          #+#    #+#             */
/*   Updated: 2024/11/08 13:49:44 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_ENUM_H
# define MINISHELL_ENUM_H

typedef enum	e_type_token
{
	INTERNAL_COMMAND,
	EXTERNAL_COMMAND,
	OPTION,
	ARGUMENT,
	STRING,
	WILDCARD,
	VARIABLE,
	REDIRECTION,
	PIPE,
	COMMAND_SUBSTITUTION,
	OPERATOR,
	CONDITIONAL_AND,
	CONDITIONAL_OR,
	BACKGROUND_COMMAND,
	COMMAND_SEQUENCE,
	ESCAPE,
	QUOTE,
	E_SPACE,
	END,
	PATH
}	t_type_token;

#endif
