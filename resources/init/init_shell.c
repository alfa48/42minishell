/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:06:54 by manandre          #+#    #+#             */
/*   Updated: 2025/01/21 11:49:01 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*parse_command(char *command, t_node *root, int side)
{
	int				i;
	char			*sep_position[4];
	t_node			*node;
	t_parseFuncs	func_array;

	if (!command)
		return (NULL);
	i = -1;
	func_array = init_parse_funcs();
	sep_position[0] = mini_strstr(command, ">>");
	sep_position[1] = mini_strstr(command, "<<");
	sep_position[2] = ft_strchr(command, '>');
	sep_position[3] = ft_strchr(command, '<');
	while (++i < 4)
	{
		if (sep_position[i])
			return (func_array.parsers[i + 1](command, root, side));
	}
	node = create_node(NULL, mini_epur_str(command), 0);
	return (add_node(root, node, side));
}

t_node	*parse_command_(char *command, t_node *root, int side)
{
	char			*sep_position;
	t_node			*node;
	t_parseFuncs	func_array;

	if (!command)
		return (NULL);
	func_array = init_parse_funcs();
	sep_position = ft_strchr(command, '|');
	if (sep_position)
		return (func_array.parsers[0](command, root, side));
	node = create_node(NULL, mini_epur_str(command), 0);
	return (add_node(root, node, side));
}

t_node	*process_pipes(char *command_line)
{
	t_node	*root;
	char	*sep;

	root = NULL;
	sep = ft_strchr(command_line, '|');
	while (sep)
	{
		if (!is_within_quotes(command_line, sep))
		{
			root = init_root_pipe(command_line);
			break ;
		}
		sep = ft_strchr(sep + 1, '|');
	}
	return (root);
}

t_node	*process_redirects(char *command_line)
{
	t_node	*root;
	char	*sep;

	root = NULL;
	sep = ft_strchr(command_line, '>');
	while (sep)
	{
		if (!is_within_quotes(command_line, sep))
		{
			root = init_root_redirec(command_line);
			return (root);
		}
		sep = ft_strchr(sep + 1, '>');
	}
	sep = ft_strchr(command_line, '<');
	while (sep)
	{
		if (!is_within_quotes(command_line, sep))
		{
			root = init_root_redirec(command_line);
			break ;
		}
		sep = ft_strchr(sep + 1, '<');
	}
	return (root);
}

t_node	*init_shell(char *command_line)
{
	t_node	*root;

	if (!command_line || ft_strlen(command_line) < 1)
		return (NULL);
	root = process_pipes(command_line);
	if (!root && has_redirect(command_line))
		root = process_redirects(command_line);
	if (!root)
		root = create_node(NULL, mini_epur_str(command_line), 0);
	return (root);
}
