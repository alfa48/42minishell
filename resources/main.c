/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:01:26 by fjilaias          #+#    #+#             */
/*   Updated: 2024/11/20 15:06:09 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tree(t_node *root)
{
	if (root == NULL)
		return ;
	free_tree(root->left);
	if (root->command)
		free(root->command);
	free_tree(root->right);
	free(root);
}

char	*mini_epur_str(char *str)
{
	char	out[42000];
	int	i;

	i = 0;
	while (*str == ' ' || *str == '\t')
		str ++;
	while (*str)
	{
		out[i++] = *str;
		if (*str++ == ' ')
		{
			while (*str == ' ' || *str == '\t')
				str ++;
			if (!*str)
				i --;
		}
	}
	out[i] = '\0';
	if (i > 0)
		return (ft_strdup(out));
	else
		return (NULL);
}

char	*mini_strstr(const char *haystack, const char *needle)
{
	const char	*h;
	const char	*n;

	if (!*needle)
		return ((char *)haystack);
	while (*haystack)
	{
		h = haystack;
		n = needle;
		while (*h && *n && *h == *n)
		{
			h ++;
			n ++;
		}
		if (!*n)
			return ((char *)haystack);
		haystack ++;
	}
	return (NULL);
}

t_node	*create_node(char *operator, char *command, int index)
{
	t_node *new;

	new = malloc(sizeof(t_node));
	if (new)
	{
		new->index = index;
		new->operator = operator;
		new->command = command;
		new->left = NULL;
		new->right  =NULL;
	}
	return (new);
}

t_node	*add_node(t_node *root, t_node *new, int side)
{	
	if (root)
	{
		if (side == 0)
			root->left = new;
		else
			root->right = new;
	}
	return (new);
}

t_node*	init_shell(char *command_line)
{//usar array de funcoes
	t_node	*root;
	t_node	*tmp;
	char	*sep_position[5];

	root = NULL;
	tmp = NULL;
	if (command_line)
	{
		sep_position[0] = ft_strchr(command_line, '|');
		sep_position[1] = mini_strstr(command_line,  ">>");
		sep_position[2] = mini_strstr(command_line, "<<");
		sep_position[3] = ft_strchr(command_line, '>');
		sep_position[4] = ft_strchr(command_line, '<');
		if (sep_position[0])
		{
			root = create_node("|", NULL, 0);
			tmp = root;
			*sep_position[0] = '\0';
			parse_command(command_line, root, 0);
			parse_command(sep_position[0] + 1, root, 1);
		}
		else if (sep_position[1])
		{
			root = create_node(">>", NULL, 0);
			tmp = root;
			*sep_position[1] = '\0';
			parse_command(command_line, root, 0);
			parse_command(sep_position[1] + 2, root, 1);
		}
		else if (sep_position[2])
		{
			root = create_node("<<", NULL, 0);
			tmp = root;
			*sep_position[2] = '\0';
			parse_command(command_line, root, 0);
			parse_command(sep_position[2] + 1, root, 1);
		}
		else if (sep_position[3])
		{
			root = create_node(">", NULL, 0);
			tmp = root;
			*sep_position[3] = '\0';
			parse_command(command_line, root, 0);
			parse_command(sep_position[3] + 1, root, 1);
		}
		else if (sep_position[4])
		{
			root = create_node("<", NULL, 0);
			tmp = root;
			*sep_position[4] = '\0';
			parse_command(command_line, root, 0);
			parse_command(sep_position[4] + 1, root, 1);
		}
		else
			root = create_node(NULL, mini_epur_str(command_line), 0);
		if (tmp)
			root = tmp;
	}
	return (root);
}

t_node*	parse_command(char *command, t_node *root, int side)
{
	t_node	*node;
	char	*pipe_position;
	char	*redir_position;

	pipe_position = strchr(command, '|');
	if (pipe_position)
	{
		*pipe_position = '\0';
		node = create_node("|", NULL, 0);
		node = add_node(root, node, side);
		parse_command(command, node, 0);
		parse_command(pipe_position + 1, node, 1);
        	return (node);
        }
	redir_position = strstr(command, ">>");
	if (redir_position)
	{
		*redir_position = '\0';
		node = create_node(">>", NULL, 0);
		node = add_node(root, node, side);
		parse_command(command, node, 0);
		parse_command(redir_position + 2, node, 1);
        	return (node);
        }
	redir_position = strstr(command, "<<");
	if (redir_position)
	{
		*redir_position = '\0';
		node = create_node("<<", NULL, 0);
		node = add_node(root, node, side);
		parse_command(command, node, 0);
		parse_command(redir_position + 2, node, 1); 
		return (node);
	}
	redir_position = strchr(command, '>');
	if (redir_position)
	{
		*redir_position = '\0';
		node = create_node(">", NULL, 0);
		node = add_node(root, node, side);
		parse_command(command, node, 0);
		parse_command(redir_position + 1, node, 1);
		return (node);
        }
	redir_position = strchr(command, '<');
	if (redir_position)
	{
		*redir_position = '\0';
		node = create_node("<", NULL, 0);
		node = add_node(root, node, side);
		parse_command(command, node, 0);
		parse_command(redir_position + 1, node, 1);
		return (node);
    }
	node = create_node(NULL, mini_epur_str(command), 0);
	node = add_node(root, node, side);
	return (node);
}

// --------------------------------------------- //

void	preorder_traversal(t_node *root)
{
	if (root == NULL)
		return ;
	if (root->operator)
		printf("Operator: %s\n", root->operator);
	else
		printf("Command: %s\n", root->command);
	preorder_traversal(root->left);
	preorder_traversal(root->right);
}

void	inorder_traversal(t_node *root)
{
	if (root == NULL)
		return ;
	inorder_traversal(root->left);
	if (root->operator)
		printf("Operator:%s$\n", (root->operator));
	else
		printf("Command:%s$\n", (root->command));
	inorder_traversal(root->right);
}

void	postorder_traversal(t_node *root)
{
	if (root == NULL)
		return ;
	postorder_traversal(root->left);
	postorder_traversal(root->right);
	if (root->operator)
		printf("Operator: %s\n", root->operator);
	else
		printf("Command: %s\n", root->command);
}

int	node_nbr(t_node *root)
{
	if (root == NULL)
		return (0);
	return (1 + node_nbr(root->left) + node_nbr(root->right));
}

int	is_it_repeated(char *name, const char *value, t_env_var *current)
{
	while (current)
	{
        if (ft_strcmp(current->name, name) == 0)
		{
        	free(current->value);
			current->value = ft_strdup(value);
            if (!current->value)
				return (0 * printf("Failed to allocate memory\n"));
            return (42);
        }
        current = current->next;
    }
	return (0);
}

int	set_it(char *name, const char *value, t_env_var **g_env_list)
{
	t_env_var	*current;
	t_env_var	*new_var;

	current = *g_env_list;
	if (is_it_repeated(name, value, current) == 42)
		return (42);
	new_var = malloc(sizeof(t_env_var));
	if (!new_var)
        return (0 * printf("Failed to allocate memory\n"));
    new_var->name = ft_strdup(name);
    if (value)
		new_var->value = ft_strdup(value);
	else
		new_var->value = ft_strdup("''");
    if (!new_var->name || !new_var->value)
		return (0 * printf("Failed to allocate memory\n"));
    new_var->next = *g_env_list;
    *g_env_list = new_var;
	return (1);
}

void	set_or_add_env_var(const char *env_entry, t_env_var **g_env_list)
{
	char	name[256];
    char	*equal_sign;
	const char	*value;
	size_t	name_len;

	equal_sign = ft_strchr(env_entry, '=');
	if (equal_sign)
	{
		name_len = equal_sign - env_entry;
		ft_strlcpy(name, env_entry, name_len);
		name[name_len] = '\0';
		value = equal_sign + 1;
		if (set_it(name, value, g_env_list) == 42)
			return ;
	}
	else
	{
		name_len = ft_strlen(env_entry) + 1;
		ft_strlcpy(name, env_entry, name_len);
		name[name_len] = '\0';
		if (set_it(name, "''", g_env_list) == 42)
			return ;
	}
}

void	initialize_env_list(t_env_var **g_env_list, char **envp)
{
	int	i;

	i = 0;
    while (envp[i])
	{
        set_or_add_env_var(envp[i], g_env_list);
		i ++;
    }
}

int	main(void)
{
	t_cmd	*cmd;
	extern char **environ;

	cmd = malloc(sizeof(t_cmd));
	init_args(cmd);
	initialize_env_list(&(cmd->g_env_list), environ);
	if (!cmd->g_env_list)
		return (0 * printf("Error in create env list!\n"));
	while (1)
	{
		cmd->line = readline("\033[36mminishell$> \033[0m");
		if (cmd->line || cmd->line[0] != '\0')
		{
			cmd->root = init_shell(cmd->line);
			if (cmd->root)
			{
				init_args_next(cmd);
				if (cmd->size == 1)
					mini_built_in(cmd, &(cmd->g_env_list));
				else
					traverse_tree(cmd->root, cmd->array, cmd->size);
				free_tree(cmd->root);
			}
			else
				printf("Erro ao criar a árvore!\n");
			cmd->index = 0;
			free_ms(cmd);
			free(cmd->line);
		}
	}
	return (0);
}
