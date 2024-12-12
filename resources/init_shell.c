/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:07:11 by fjilaias          #+#    #+#             */
/*   Updated: 2024/12/10 08:06:25 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_funcArray	init_func_array(void)
{
	t_funcArray	func_array;

	func_array.handlers[0] = handle_pipe;
	func_array.handlers[1] = handle_double_right;
	func_array.handlers[2] = handle_double_left;
	func_array.handlers[3] = handle_single_right;
	func_array.handlers[4] = handle_single_left;
	return (func_array);
}

static t_parseFuncs	init_parse_funcs(void)
{
	t_parseFuncs	func_array;

	func_array.parsers[0] = parse_pipe;
	func_array.parsers[1] = parse_double_right;
	func_array.parsers[2] = parse_double_left;
	func_array.parsers[3] = parse_single_right;
	func_array.parsers[4] = parse_single_left;
	return (func_array);
}

t_node	*init_shell(char *command_line)
{
	int		i;
	char	*sep_position[5];
	t_node	*root;
	t_funcArray	func_array;

	if (!command_line || ft_strlen(command_line) < 1)
		return (NULL);
	root = NULL;
	func_array = init_func_array();
	sep_position[0] = ft_strchr(command_line, '|');
	sep_position[1] = mini_strstr(command_line, ">>");
	sep_position[2] = mini_strstr(command_line, "<<");
	sep_position[3] = ft_strchr(command_line, '>');
	sep_position[4] = ft_strchr(command_line, '<');
	i = -1;
	while (++ i < 5)
	{
		if (sep_position[i])
			return (func_array.handlers[i](command_line));
	}
	if (!root)
		root = create_node(NULL, mini_epur_str(command_line), 0);
	return (root);
}

t_node	*parse_command(char *command, t_node *root, int side)
{
	int		i;
	char		*sep_position[5];
	t_node	*node;
	t_parseFuncs	func_array;

	if (!command)
		return (NULL);
	i = -1;
	func_array = init_parse_funcs();
	sep_position[0] = ft_strchr(command, '|');
	sep_position[1] = mini_strstr(command, ">>");
	sep_position[2] = mini_strstr(command, "<<");
	sep_position[3] = ft_strchr(command, '>');
	sep_position[4] = ft_strchr(command, '<');
	while (++ i < 5)
	{
		if (sep_position[i])
			return (func_array.parsers[i](command, root, side));
	}
	node = create_node(NULL, mini_epur_str(command), 0);
	return (add_node(root, node, side));
}

