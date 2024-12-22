/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:24:48 by manandre          #+#    #+#             */
/*   Updated: 2024/12/13 09:10:52 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*mini_strcat(char *dest, const char *src)
{
	char    *p_dest;

	p_dest = dest;
	while (*p_dest != '\0')
		p_dest ++;
	while (*src != '\0')
	{
		*p_dest = *src;
		p_dest ++;
		src ++;
	}
	*p_dest = '\0';
	return (dest);
}

static int	get_args_size(char **cmd_args)
{
	int	size;

	size = 0;
	while (cmd_args[size] != NULL)
		size ++;
	return (size);
}

char	**get_args(char *cmd)
{
	char	**cmd_args;
	char	**execve_args;
	int		i;

	i = 0;
	cmd_args = ft_split(cmd, ' ');
	execve_args = malloc((get_args_size(cmd_args) + 1) * sizeof(char *));
	execve_args[0] =  ft_strjoin("/bin/", cmd_args[0]);
	while (++i < get_args_size(cmd_args))
		execve_args[i] = cmd_args[i];
	execve_args[i] = NULL;
	return (execve_args);
}

void	execute_command(t_node *node, char **env)
{
	char	**execve_args;

	pid_t pid = fork();
	if (pid == 0)
	{
		execve_args = get_args(node->command);
		fprintf(stderr, "Debug: Comando full %s\n", execve_args[0]);
		execve(execve_args[0], execve_args, env);
		perror("execve failed");
		exit(1);
	}
	waitpid(pid, NULL, 0);
}

void	execute_tree(t_node *root, char **env, t_cmd *cmd)
{
	if (!root)
		return ;
	fprintf(stderr, "Debug: Executando comando %s\n", root->command ? root->command : root->operator);
	if (root->command != NULL)
		execute_command(root, env);
	else if (root->operator && ft_strcmp(root->operator, "|") == 0)
		execute_pipe(root, env, cmd);
	else if (root->operator != NULL)
		execute_redirect(root, env, cmd);	
}

void    exec(t_cmd *cmd, char **env)
{
	t_node	*tmp_root = cmd->root;
	(void)env;
	fprintf(stderr, "Debug: root: %s\n", cmd->root->operator);
	execute_tree(tmp_root, env, cmd);
}

/*Agora eu queria que ideias de como eu posso depois de ler e ter arganizada todos os possíveis nós, percorrer a árvore no ato de executar o comando, considerando se depois de um comando há um pipe ou outro redirecionador, se antes de um comando foi deixado um pipe, e assim por diante. Ca tenho funcões que o meu projeto em C permite usar : "readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history, printf, malloc, free, write, access, open, read, close, fork, wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset, sigaddset, kill, exit, getcwd, chdir, stat, lstat, fstat, unlink, execve, dup, dup2, pipe, opendir, readdir, closedir, strerror, perror, isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs"
*/
