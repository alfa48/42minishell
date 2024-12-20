/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 08:52:19 by fjilaias          #+#    #+#             */
/*   Updated: 2024/12/13 10:42:16 by fjilaias         ###   ########.fr       */
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
#include <linux/limits.h>
# include "minishell_enum.h"
# include "structs.h"
# include "libft.h"

# define BUFFER_SIZE 3
# define FILE_MODE_WRITE O_WRONLY | O_CREAT | O_TRUNC
# define FILE_MODE_READ O_RDONLY

//function_one.c
char	*ft_concat_char(char *str, char c);
char	*rm_last_char(char *str);

//base/input.c
void	tread_fwrite(t_cmd_line *cmd);

//base/bread.c
int	open_fread(void);
char	gnc(int fd);

//buffer/init_buffer.c
void	init_buffers(t_cmd_line *cmd);
void	add_pointer(t_cmd_line *cmd);
void	gpc_buffer(t_cmd_line *cmd);
void	load_buffer1(t_cmd_line *cmd);
void    load_buffer2(t_cmd_line *cmd);

//buffer/io_buffer.c
char	gnc_buffer(t_cmd_line *cmd);

//token/check.c
t_token	*check_redirection(char c, t_cmd_line *c_line);
t_token *check_condicitional(t_cmd_line *c_line);
t_token *check_keyword(t_cmd_line *cmd);
t_token	*check_special_characters(t_cmd_line *cmd);
t_token	*check_end(t_cmd_line *cmd);

//token/tokens_aux.c
t_token	*check_env(t_cmd_line *cmd);
t_token	*check_hifem(t_cmd_line *cmd);
t_token *check_litle_high_commas(t_cmd_line *cmd);
t_token *check_redirectors(t_cmd_line *cmd);
t_token	*check_ex_cmd(t_cmd_line *cmd);
t_token *check_high_commas(t_cmd_line *cmd);
t_token	*check_relative_path(t_cmd_line *cmd);
t_token	*check_absolutely_path(t_cmd_line *cmd);

//token/check_utils.c
void	check_space(t_cmd_line *cmd);
t_token *check_pipe_or(t_cmd_line *cmd);

//token/final_checkers/space_end.c
t_token *end(t_cmd_line *cmd, t_token *token);
t_token *condicitional(t_cmd_line *cmd, t_token *token);

//token/lexs.c
void	ft_clear(t_cmd_line *cmd);
void	ft_enter(t_cmd_line *cmd);

//token/tokens.c
t_token *create_token(int type, char *lexema);
t_token *next_token(t_cmd_line *c_line);



//------------------------------------------------------------------------------



//resources/main.c
int main(void);

//resources/mini_execv.c
void	mini_val(char *str, t_env_var **g_env_list);
void only_expor_cmd(t_env_var *g_env_list);
char    *get_word(char *line, int *sig);
void    list_env_vars(t_env_var *g_env_list);
void    mini_built_in(t_cmd *cmd, t_env_var **g_env_list);

//resources/init/init_vars
int calc_tree_size(t_node *root);
void    init_args(t_cmd  *cmd);
void    init_args_next(t_cmd  *cmd);
void	fill_array_cmd(t_node *root, char **array, int *index);

//resources/utils/mini_utils.c
char    **get_args(char *cmd);
char    *mini_strcat(char* dest, const char* src);
void    exec(t_cmd *cmd, char **env);
void	traverse_tree(t_node *root, char **array, int size, t_env_var *g_env_list);
void	*ft_memset_space(void *ptr, int value, size_t num);

//resources/utils/mini_utils_2.c
void	free_tree(t_node *root);
char	*mini_epur_str(char *str);
t_node	*create_node(char *operator, char *command, int index);
t_node	*add_node(t_node *root, t_node *new, int side);
int is_special_char(char c);
int is_echo_printable(char c);

//resources/utils/mini_utils_3.c
char	*mini_strstr(const char *haystack, const char *needle);
void	set_or_add_env_var(const char *env_entry, t_env_var **g_env_list);
void	initialize_env_list(t_env_var **g_env_list, char **ev);
int	set_it(char *name, const char *value, t_env_var **g_env_list);

//resources/mini_expand.c
char *concat_strings(char **str_array);
char *expanding(char *str, t_env_var *g_env_list);

//token/cmd_in/cmd_envexit.c
void    free_ms(t_cmd *cmd);
void    mini_env(t_env_var *g_env_list);
void    mini_exit(void);

//resources/cmd_cepeu.c
bool check_quotes_balance(const char *arg);
void    mini_echo(char *arg);
void    mini_cd(char *path, t_env_var *g_env_list);
void    mini_pwd(void);
void    mini_export(char **args, t_env_var **g_env_list);
void    mini_unset(char **args, t_env_var **g_env_list);
char    *ft_findenv(char *s, t_env_var *g_env_list);

//resources/first_handler.c
t_node	*handle_pipe(char *command_line);
t_node	*handle_double_right(char *command_line);
t_node	*handle_double_left(char *command_line);
t_node	*handle_single_right(char *command_line);
t_node	*handle_single_left(char *command_line);

//resources/parse_cmd.c
t_node	*parse_pipe(char *command, t_node *root, int side);
t_node	*parse_double_right(char *command, t_node *root, int side);
t_node	*parse_double_left(char *command, t_node *root, int side);
t_node	*parse_single_right(char *command, t_node *root, int side);
t_node	*parse_single_left(char *command, t_node *root, int side);

//resources/init_shell.cls
t_node	*init_shell(char *command_line);
t_node	*parse_command(char *command, t_node *root, int side);


# endif
