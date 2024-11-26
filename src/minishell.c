/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 08:51:21 by fjilaias          #+#    #+#             */
/*   Updated: 2024/11/18 11:53:46 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
static void	get_string(t_token *token, t_cmd_line *c_line)
{
	(void) c_line;
	(void) token;
	printf("token {%d, %s}\n", token->type, token->lexema);
}
*/

/*int	n_main(void)
{
	t_token	*token;
	t_cmd_line	*cmd;
	char	*argv[] = { NULL };
	char	*envp[] = { "PATH=/bin", "TERM=xterm" , NULL };
	char	**args;
	pid_t	p;

	(void)token;
	cmd = malloc(sizeof(t_cmd_line));
	args = (char **)malloc(sizeof(char *) * 4);
	cmd->index = 0;
	int j = 0;
	while (j < 5)
	{
		tread_fwrite(cmd);
		init_buffers(cmd);
		int	i = 0;
		while(i < 3)
		{
			args[i] = next_token(cmd)->lexema;
			printf("%s\n", args[i]);
			i ++;
		}
		//printf("Um print %sDoze\n", args[0]);
		if (ft_strcmp("cd", args[0]) == 0)
		{
			mini_cd(args[1]);
			//mini_pwd();
		}
		else if (ft_strcmp("echo", args[0]) == 0)
			mini_echo(args);
		else if (ft_strcmp("pwd", args[0]) == 0)
			mini_pwd();
		else if (ft_strcmp("exit", args[0]) == 0)
			mini_exit();
		else if (ft_strcmp("export", args[0]) == 0)
			mini_export(ft_strjoin(args[1], args[2]));
		else if (ft_strcmp("ls", args[0]) == 0)
		{
			p = fork();
			if (p == 0)
				execve("/bin/ls", argv, envp);
			waitpid(p, NULL, 0);
		}
		else if (ft_strcmp("env", args[0]) == 0)
		{
			p = fork();
			if (p == 0)
				execve("/bin/env", argv, envp);
			//waitpid(p, NULL, 0);
		}
		j ++;
	}
	free_ms(args);
	return (0);
}
*/







/*token = next_token(cmd);
    if (token)
    	get_string(token, cmd);
    token = next_token(cmd);
    if (token)
    	get_string(token, cmd);
    token = next_token(cmd);
    if (token)
    	get_string(token, cmd);
	free(cmd->lexema);
	free(cmd);
	close(cmd->fd);*/
