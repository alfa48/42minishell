/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 08:28:30 by manandre          #+#    #+#             */
/*   Updated: 2025/01/21 15:40:24 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	STRUCTS_H
# define STRACTS_H

typedef struct	s_token
{
	t_type_token type;
	char *lexema;
}	t_token;

typedef struct s_token_map
{
	char c;
	int type;
	char *lexema;
}	t_token_map;

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
	int		size;
	char	**array;
	char    **array_redirect;
	char	*line;
	char	*line1;
	char	**arg;
	char	**envl;
	int		index;
	int		nl;
	int		ncmd;
	int		status_cmd_prev;
    int		pid_count;
	int	pipefd[2];//pipe de entrada
	int prev_pipe[2]; // Pipe anterio
	int status_cmd;
	bool in_s_q;
    bool in_d_q;
	t_env_var 	*val_only;
	t_env_var	*g_env_list;

}	t_cmd;

typedef struct s_funcArray
{
	t_node	*(*handlers[5])(char *command_line);
}	t_funcArray;

typedef struct s_parseFuncs
{
	t_node *(*parsers[5])(char *, t_node *, int);
}	t_parseFuncs;

typedef struct s_redirect {
    char *type;      // "<", ">", or ">>"
    char *file;      // Nome do arquivo
    int fd;          // File descriptor
} t_redirect;

#endif
