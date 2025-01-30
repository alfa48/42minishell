/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 08:52:19 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/31 00:32:52 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "structs.h"
# include <errno.h>
# include <fcntl.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# define MAX_REDIRECTS 10
# define PIPE '|'

// global variable
extern int		g_signal_status;

// resources/main/main.c
int				main(void);

// resources/mini/mini_built_ins.c
void			only_expor_cmd(t_env_var *g_env_list);
void			list_env_vars(t_env_var *g_env_list);
void			mini_built_in(t_cmd *cmd, t_env_var **g_env_list);
void			ft_qsort(void *base, size_t num, size_t size,
					int (*compare)(const void *, const void *));

// resources/init/init_vars
int				calc_tree_size(t_node *root);

// void    init_args(t_cmd  *cmd);
void			init_args_ofen(t_cmd *cmd);
void			init_args(t_cmd *cmd, char **environ);
void			init_args_next(t_cmd *cmd);
void			fill_array_cmd(t_node *root, char **array, int *index);

// esources/utils/heredoc.c
void			exec_heredoc(t_node *node, char **env, t_cmd *cmd);

// resources/utils/exec_pipe_redirectrs.c
char			*remove_redirects(const char *cmd);

// resources/utils/mini_utils.c
void			execute_tree(t_node *root, t_cmd *cmd);
void			exec_redin(t_node *node, char **env, t_cmd *cmd);
char			**get_args(char *cmd);
int				is_operator(char *str);
int				is_redirect(char *str);
char			*mini_strcat(char *dest, const char *src);
void			execute_commands(int pos, t_cmd *cmd);

// resources/exec/mini_exec.c
void			exec(t_cmd *cmd);
void			cmd_not_found_end_exit(char *str);

// resources/utils/mini_utils_2.c
void			free_tree(t_node *root);
char			*mini_epur_str(char *str);
t_node			*create_node(char *operator, char *command, int index);
t_node			*add_node(t_node *root, t_node *new, int side);

// resources/utils/mini_utils_3.c
char			*mini_strstr(const char *haystack, const char *needle);
void			set_or_add_env_var(const char *env_entry,
					t_env_var **g_env_list);
void			initialize_env_list(t_env_var **g_env_list, char **ev);
int				set_it(char *name, const char *value, t_env_var **g_env_list);

// resources/pipe/exec_pipe_middle.c
void			execute_pipe_middle_(int pos, t_cmd *cmd);

// resources/pipe/exec_pipe_right.c
void			execute_pipe_right(int pos, t_cmd *cmd);
void			handle_redirects(t_redirect **redirects);
char			*aux_heredoc_right(char *cmd, int pipefd[2]);

// resources/pipe/exec_pipe_left.c
void			execute_pipe_left(int pos, t_cmd *cmd);
void			configure_stdin(char *heredoc_delim, int *pipefd);
char			*prepare_command(char *cmd, char *heredoc_delim);
void			mini_close_fd(int fd_0, int fd_1);

// resources/pipe/utils.c
void			setup_io(t_redirect **redirects, int *prev_pipe, int *next_pipe,
					bool is_middle);
bool			has_output_redirect(t_redirect **redirects);
bool			has_input_redirect(t_redirect **redirects);
void			error_execve(char *ccmd, char *path, char **args);
void			execute_with_args(char *clean_cmd, t_redirect **redirects,
					t_cmd *cmd);
void			execute_redirect_(int pos, t_cmd *cmd);
void			exec_redout_(int pos, t_cmd *cmd);
void			apply_file_redirections(t_redirect **r);

// resources/utils/mini_utils.c
void			execute_tree(t_node *root, t_cmd *cmd);
void			exec_redin(t_node *node, char **env, t_cmd *cmd);
char			**get_args(char *cmd);
int				is_operator(char *str);
int				is_redirect(char *str);
char			*mini_strcat(char *dest, const char *src);

// resources/exec/exec_single_commands.c
int				execute_single_command(char *cmd_str, t_cmd *cmd);
void			execute_child_process(t_redirect **redirects, t_cmd *cmd);

// resources/exec/exec_single_commands_utils.c
void			exit_child_process(t_cmd *cmd, char *path, char **args,
					t_fd_data *fd_data);
int				apply_redirects(t_redirect **redirects, int *opened_fds,
					int *fd_count);

// resources/exec/exec_commands.c
void			execute_commands(int pos, t_cmd *cmd);

// resources/exec/exec_pipe_redirectrs.c
void			free_redirects(t_redirect **redirects);
t_redirect		**parse_redirects(char *cmd_str, t_cmd *cmd);

// resources/exec/exec_utils_2.c
int				is_redirect_char(char c);
void			free_array(char **array);

// resources/utils/mini_utils_4.c
char			*get_env_var(char *name, t_env_var *env_list);
char			*get_first_word(const char *line);
void			cmd_not_found(char *str);
int				is_within_quotes(char *str, char *sep);
int				is_entirely_within_quotes(char *str);
char			*ft_strncpy(char *dst, const char *src, size_t n);

// resources/utils/mini_utils_5.c
int				is_valid_env_var_name(const char *str);

// resources/utils/mini_utils_6.c
char			*get_word(char *line, int *sig, char *sigline);
int				mini_isspace(int c);
void			*ft_realloc(void *ptr, size_t new_size);

// resources/utils/mini_utils_7.c
char			*get_first_word(const char *line);

// resources/mini_expand.c
char			*expanding(char *str, t_cmd *cmd);
char			*ft_strndup(const char *s, size_t n);

// token/cmd_in/cmd_envexit.c
void			free_ms(t_cmd *cmd);
void			mini_env(t_env_var *g_env_list);
void			mini_exit(t_cmd *cmd);

// resources/cmd_cepeu.c
bool			check_quotes_balance(const char *arg);
void			mini_echo(char *arg, char *sigline);
void			mini_cd(char *path, t_env_var *g_env_list);
void			mini_pwd(void);
void			mini_export(char **args, t_env_var **g_env_list);
void			mini_unset(char **args, t_env_var **g_env_list);
char			*ft_findenv(char *s, t_env_var *g_env_list);
char			*process_cmd(char *arg);
void			execute_in_child(char *path, char *args[]);

// resources/first_handler.c
t_node			*handle_pipe(char *command_line);
t_node			*handle_double_right(char *command_line);
t_node			*handle_double_left(char *command_line);
t_node			*handle_single_right(char *command_line);
t_node			*handle_single_left(char *command_line);

// resources/parse_cmd.c
t_node			*parse_pipe(char *command, t_node *root, int side);
t_node			*parse_double_right(char *command, t_node *root, int side);
t_node			*parse_double_left(char *command, t_node *root, int side);
t_node			*parse_single_right(char *command, t_node *root, int side);
t_node			*parse_single_left(char *command, t_node *root, int side);

// resources/init_shell.cls
t_node			*init_shell(char *command_line);
int				has_redirect(char *command);
t_node			*parse_command(char *command, t_node *root, int side);
t_node			*parse_command_(char *command, t_node *root, int side);
t_node			*init_root_pipe(char *command_line);
t_node			*init_root_redirec(char *command_line);
t_parseFuncs	init_parse_funcs(void);
t_funcArray		init_func_array(void);

// resources/init_before.c
t_cmd			*init_before_init(void);

// resources/signals/cmd_signal.c
void			handle_signals(void);
void			sigint_handler(int signum);
void			set_sig_status_cmd(t_cmd *cmd);
void			handle_signals_child(void);

// resources/path/path_utils.c
char			*find_executable(const char *command, t_env_var **g_env_list);

// resources/forks/cmd_forks.c
void			fork_crt_env_vars(t_cmd *cmd);
void			wait_forks(t_cmd *cmd);
void			fork_exec_cmd(t_cmd *cmd, t_node *node);
char			*aux_exec(char *ccmd, t_cmd *cmd);

// resources/redirect/redirect.c
char			**ft_split_redirect(const char *str);
void			exec_command_redirect(int pos, t_cmd *cmd);

// resources/heredoc/util.c
char			*get_heredoc_delimiter(char *cmd);
char			*remove_heredoc(char *cmd);
void			mini_heredoc(t_cmd *cmd);

// resources/checks/checks.c
int				checks_error_pattern(char *texto);
int				checks_str(t_cmd *cmd);

// resources/heredoc/util.c
char			*get_heredoc_delimiter(char *cmd);
char			*remove_heredoc(char *cmd);
void			handle_heredoc(char *delimiter, int fd_destino[2]);
void			mini_heredoc(t_cmd *cmd);

// resources/checks/checks.c
int				checks_error_pattern(char *texto);
int				checks_str(t_cmd *cmd);

// resources/heredoc/utils.c
void			handle_heredoc_left(char *delimiter, int pipe_fd[2]);

// resources/free_up/free_inits.c
void			free_env_list(t_env_var **g_env_list);
int				write_exit(t_cmd *cmd);
void			free_lines(t_cmd *cmd);
void			free_arg(t_cmd *cmd);
void			free_cmd_array(t_cmd *cmd);
void			free_ms(t_cmd *cmd);
void			free_tree(t_node *root);
void			free_all(t_cmd *cmd);
void			free_one_iterator(t_cmd *cmd);
int				free_unset(t_env_var *current);

// resouces/mini_split/mini_split.c
char			**mini_split(char const *s, char c);

#endif
