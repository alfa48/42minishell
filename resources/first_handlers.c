/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:07:11 by fjilaias          #+#    #+#             */
/*   Updated: 2024/12/02 11:46:15 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*handle_pipe(char *command_line)
{
	char	*sep = ft_strchr(command_line, '|');
	t_node	*root = create_node("|", NULL, 0);

	*sep = '\0';
	parse_command(command_line, root, 0);
	parse_command(sep + 1, root, 1);
	return (root);
}

t_node	*handle_double_right(char *command_line)
{
	char	*sep = mini_strstr(command_line, ">>");
	t_node	*root = create_node(">>", NULL, 0);

	*sep = '\0';
	parse_command(command_line, root, 0);
	parse_command(sep + 2, root, 1);
	return (root);
}

t_node	*handle_double_left(char *command_line)
{
	char	*sep = mini_strstr(command_line, "<<");
	t_node	*root = create_node("<<", NULL, 0);

	*sep = '\0';
	parse_command(command_line, root, 0);
	parse_command(sep + 2, root, 1);
	return (root);
}

t_node	*handle_single_right(char *command_line)
{
	char	*sep = ft_strchr(command_line, '>');
	t_node	*root = create_node(">", NULL, 0);

	*sep = '\0';
	parse_command(command_line, root, 0);
	parse_command(sep + 1, root, 1);
	return (root);
}

t_node	*handle_single_left(char *command_line)
{
	char	*sep = ft_strchr(command_line, '<');
	t_node	*root = create_node("<", NULL, 0);

	*sep = '\0';
	parse_command(command_line, root, 0);
	parse_command(sep + 1, root, 1);
	return (root);
}
