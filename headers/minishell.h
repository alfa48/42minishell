/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 08:52:19 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/07 10:03:12 by fjilaias         ###   ########.fr       */
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

# define BUFFER_SIZE 5
# define FILE_MODE_WRITE O_WRONLY | O_CREAT | O_TRUNC
# define FILE_MODE_READ O_RDONLY
# define MAX_REDIRECTS 3 

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
void	mini_val(char *str, t_cmd *cmd);
void	only_expor_cmd(t_env_var *g_env_list);
char    *get_word(char *line, int *sig);
void    list_env_vars(t_env_var *g_env_list);
void    mini_built_in(t_cmd *cmd, t_env_var **g_env_list);

//resources/init/init_vars
int calc_tree_size(t_node *root);
//void    init_args(t_cmd  *cmd);
void    init_args_ofen(t_cmd  *cmd);
void    init_args(t_cmd  *cmd, char **environ);
void    init_args_next(t_cmd  *cmd);
void	fill_array_cmd(t_node *root, char **array, int *index);

//esources/utils/heredoc.c
void	exec_heredoc(t_node *node, char **env, t_cmd *cmd);

//resources/utils/exec_pipe_redirectrs.c
void free_array(char **array);
void free_redirects(t_redirect **redirects);
char *remove_redirects(const char *cmd);
t_redirect **parse_redirects(char *cmd_str);
void apply_redirects(t_redirect **redirects, int prev_pipe[2], int next_pipe[2]);
void	execute_pipe(t_node *node, char **env, t_cmd *cmd);
void	execute_pipe_right(int pos, t_cmd *cmd);
void	execute_pipe_left(int pos, t_cmd *cmd);
void execute_pipe_middle(int pos, t_cmd *cmd);
void	execute_redirect(t_node *node, char **env,  t_cmd *cmd);

void	execute_redirect_(int pos,  t_cmd *cmd);
void	exec_redout_(int pos, t_cmd *cmd);

//resources/utils/mini_utils.c
void	execute_tree(t_node *root, t_cmd *cmd);
void	exec_redin(t_node *node, char **env, t_cmd *cmd);
char    **get_args(char *cmd);
int is_operator(char *str);
int is_redirect(char *str);
char    *mini_strcat(char* dest, const char* src);
void    exec(t_cmd *cmd);
void execute_commands(int pos, t_cmd *cmd);
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

//resources/utils/mini_utils_4.c
char *get_env_var(const char *name, t_env_var *env_list);
char *get_first_word(char *line);
void cmd_not_found(char *str);

//resources/mini_expand.c
char *concat_strings(char **str_array);
char *expanding(char *str, t_cmd *cmd);

//token/cmd_in/cmd_envexit.c
void    free_ms(t_cmd *cmd);
void    mini_env(t_env_var *g_env_list);
void    mini_exit(t_cmd *cmd);

//resources/cmd_cepeu.c
bool check_quotes_balance(const char *arg);
void    mini_echo(char *arg);
void    mini_cd(char *path, t_env_var *g_env_list);
void    mini_pwd(void);
void    mini_export(char **args, t_env_var **g_env_list);
void    mini_unset(char **args, t_env_var **g_env_list);
char    *ft_findenv(char *s, t_env_var *g_env_list);
char	*process_cmd(char *arg);
void execute_in_child(char *path, char *args[]);

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
int	has_redirect(char *command);
t_node	*parse_command(char *command, t_node *root, int side);
t_node	*parse_command_(char *command, t_node *root, int side);

//resources/signals/cmd_signal.c
void    handle_signals(void);
void    sigint_handler(int signum);

//resources/path/path_utils.c
char *find_executable(const char *command, t_env_var **g_env_list);

//resources/forks/cmd_forks.c
void fork_crt_env_vars(t_cmd *cmd);
void	wait_forks(t_cmd *cmd);
void    fork_exec_cmd(t_cmd *cmd, t_node *node);
void    fork_exec_cmd_(int pos, t_cmd *cmd);

//resources/redirect/redirect.c
char **ft_split_redirect(const char *str);
void exec_command_redirect(int pos, t_cmd *cmd);


# endif
