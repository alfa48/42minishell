/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 11:27:09 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/21 11:50:26 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_funcArray	init_func_array(void)
{
	t_funcArray	func_array;

	func_array.handlers[0] = handle_pipe;
	func_array.handlers[1] = handle_double_right;
	func_array.handlers[2] = handle_double_left;
	func_array.handlers[3] = handle_single_right;
	func_array.handlers[4] = handle_single_left;
	return (func_array);
}

t_parseFuncs	init_parse_funcs(void)
{
	t_parseFuncs	func_array;

	func_array.parsers[0] = parse_pipe;
	func_array.parsers[1] = parse_double_right;
	func_array.parsers[2] = parse_double_left;
	func_array.parsers[3] = parse_single_right;
	func_array.parsers[4] = parse_single_left;
	return (func_array);
}

t_node	*init_root_pipe(char *command_line)
{
	char	*sep_position;
	t_node	*root;

	root = NULL;
	sep_position = ft_strchr(command_line, '|');
	if (sep_position)
		return (handle_pipe(command_line));
	return (root);
}

int	has_redirect(char *command)
{
	char	*pos;

	pos = ft_strchr(command, '<');
	if (pos && *(pos + 1) != '<')
		return (1);
	pos = ft_strchr(command, '>');
	if (pos)
		return (1);
	return (0);
}

t_node	*init_root_redirec(char *command_line)
{
	int			i;
	char		*sep_position[4];
	t_node		*root;
	t_funcArray	func_array;

	root = NULL;
	func_array = init_func_array();
	sep_position[0] = mini_strstr(command_line, ">>");
	sep_position[1] = mini_strstr(command_line, "<<");
	sep_position[2] = ft_strchr(command_line, '>');
	sep_position[3] = ft_strchr(command_line, '<');
	i = -1;
	while (++i < 4)
	{
		if (sep_position[i])
			return (func_array.handlers[i + 1](command_line));
	}
	return (root);
}
