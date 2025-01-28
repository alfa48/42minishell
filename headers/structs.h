/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 08:28:30 by manandre          #+#    #+#             */
/*   Updated: 2025/01/28 14:31:56 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRACTS_H

typedef struct s_node
{
	char				*command;
	int					index;
	char				*operator;
	struct s_node		*left;
	struct s_node		*right;
}						t_node;

typedef struct s_env_var
{
	char				*name;
	char				*value;
	int					counter_exp;
	struct s_env_var	*next;
}						t_env_var;

typedef struct s_funcArray
{
	t_node				*(*handlers[5])(char *command_line);
}						t_funcArray;

typedef struct s_parseFuncs
{
	t_node				*(*parsers[5])(char *, t_node *, int);
}						t_parseFuncs;

typedef struct s_redirect
{
	char				*type;
	char				*file;
	int					fd;
}						t_redirect;

// Estrutura para armazenar estado de redirecionamento
typedef struct s_redirect_state
{
	bool				in_quotes;
	bool				in_redirect;
	char				quote_char;
	char				*write_ptr;
	const char			*read_ptr;
}						t_redirect_state;

typedef struct s_fd_data
{
	int					*opened_fds;
	int					fd_count;
}						t_fd_data;

typedef struct s_cmd
{
	struct s_node		*root;
	size_t				is;
	size_t				js;
	int					i;
	int					size;
	char				**array;
	char				**array_redirect;
	char				*line;
	char				*line1;
	char				**arg;
	char				**envl;
	char				*clean_cmd;
	int					index;
	int					nl;
	int					ncmd;
	int					pid_count;
	int					pipefd[2];
	int					prev_pipe[2];
	int					status_cmd;
	bool				in_s_q;
	bool				in_d_q;
	t_redirect			**redirects;
	t_env_var			*g_env_list;

}						t_cmd;

#endif
