/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 08:28:30 by manandre          #+#    #+#             */
/*   Updated: 2024/11/27 13:08:15 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	STRUCTS_H
# define STRACTS_H

typedef struct	s_cmd_line
{
	char	*buffer;
	int	corrent_buffer;
	int	pointer;
	int	fd;
	char	*line;
	char	*lexema;
	int	index;
	int	init_lex;
}	t_cmd_line;

typedef struct	s_node
{
	char	*command;
    int		index;
	char	*operator;
    struct s_node	*left;
    struct s_node	*right;
}	t_node;

typedef struct	s_env_var
{
	char	*name;
    char	*value;
	int		counter_exp;
    struct s_env_var	*next;
}	t_env_var;

typedef	struct s_cmd
{
	struct s_node	*root;
	int		size;//quantidade de elementos da arvore
	char	**array;
	char	*line;
	char	**arg;
	int		index;
	int		nl;
	int		status_cmd_prev;
	t_env_var	*g_env_list;

} t_cmd;

#endif
