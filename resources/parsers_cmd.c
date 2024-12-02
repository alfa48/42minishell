/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 11:25:08 by fjilaias          #+#    #+#             */
/*   Updated: 2024/12/02 11:25:27 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*parse_pipe(char *command, t_node *root, int side)
{
	char	*sep = strchr(command, '|');
	t_node	*node = create_node("|", NULL, 0);

	*sep = '\0';
	node = add_node(root, node, side);
	parse_command(command, node, 0);
	parse_command(sep + 1, node, 1);
	return (node);
}

t_node	*parse_double_right(char *command, t_node *root, int side)
{
	char	*sep = strstr(command, ">>");
	t_node	*node = create_node(">>", NULL, 0);

	*sep = '\0';
	node = add_node(root, node, side);
	parse_command(command, node, 0);
	parse_command(sep + 2, node, 1);
	return (node);
}

t_node	*parse_double_left(char *command, t_node *root, int side)
{
	char	*sep = strstr(command, "<<");
	t_node	*node = create_node("<<", NULL, 0);

	*sep = '\0';
	node = add_node(root, node, side);
	parse_command(command, node, 0);
	parse_command(sep + 2, node, 1);
	return (node);
}

t_node	*parse_single_right(char *command, t_node *root, int side)
{
	char	*sep = strchr(command, '>');
	t_node	*node = create_node(">", NULL, 0);

	*sep = '\0';
	node = add_node(root, node, side);
	parse_command(command, node, 0);
	parse_command(sep + 1, node, 1);
	return (node);
}

t_node	*parse_single_left(char *command, t_node *root, int side)
{
	char	*sep = strchr(command, '<');
	t_node	*node = create_node("<", NULL, 0);

	*sep = '\0';
	node = add_node(root, node, side);
	parse_command(command, node, 0);
	parse_command(sep + 1, node, 1);
	return (node);
}

