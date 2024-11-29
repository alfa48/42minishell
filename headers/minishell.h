/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 08:52:19 by fjilaias          #+#    #+#             */
/*   Updated: 2024/11/27 14:10:42 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
#include <signal.h>
#include <linux/limits.h>
# include "structs.h"
# include "libft.h"

# define BUFFER_SIZE 3
# define FILE_MODE_WRITE O_WRONLY | O_CREAT | O_TRUNC
# define FILE_MODE_READ O_RDONLY

//variaveis globais
extern int g_signal;

//cmd_in/cmd_envexit.c
void    free_ms(t_cmd *cmd);
void    mini_env(t_env_var *g_env_list);
void    mini_exit(void);

//cmd_in/cmd_cepeu.c
void    mini_echo(t_env_var *env_var, char *arg);
void    mini_cd(char *path, t_env_var *g_env_list);
void    mini_pwd(void);
void    mini_export(char **args, t_env_var **g_env_list);
void    mini_unset(char **args, t_env_var **g_env_list);


//resources/main.c
char	*mini_epur_str(char *str);
t_node	*parse_command(char *command_line, t_node *root, int side);
t_node	*create_node(char *operator, char *command, int index);
t_node	*add_node(t_node *root, t_node *new, int side);
t_node	*init_shell(char *command_line);
char	*mini_strstr(const char *haystack, const char *needle);
void	ft_error(t_cmd *cmd,char *msg, int flag);

//resources/signal/mini_execv.c
void    mini_built_in(t_cmd *cmd, t_env_var **g_env_list);
void    list_env_vars(t_env_var *g_env_list);
void	set_or_add_env_var(const char *env_entry, t_env_var **g_env_list);
void	initialize_env_list(t_env_var **g_env_list, char **ev);
char    *get_word(char *line, int *sig);

//resources/init/init_vars
int	calc_tree_size(t_node *root);
void    init_args(t_cmd  *cmd);
void    init_args_next(t_cmd  *cmd);
void	fill_array_cmd(t_node *root, char **array, int *index);

//resources/utils/mini_utils.c
char    **get_args(char *cmd);
void    exec(t_cmd *cmd, t_env_var *g_env_list);
void	traverse_tree(t_node *root, char **array, int size, t_env_var *g_env_list);

//resources/signal/mini_signal.c
void    signal_handler(int signal);
void    check_signal();



# endif
