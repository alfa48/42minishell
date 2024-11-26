/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_execv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:56:05 by fjilaias          #+#    #+#             */
/*   Updated: 2024/11/20 15:40:21 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_word(char *line, int *sig)
{
    char *init;
    char *end;
    char  *start;
    int i;

    i = -1;
    init = mini_strstr(line, "echo");
    while(init[++i] != 32);
    if (init == NULL)
        return (NULL);
    end = &init[i];
    init = mini_strstr(end, "-n");
    i = -1;
    if (!init || init[2] != 32)
    {
        while (*end == ' ' || *end == '\t')
            end ++;
        start = end;
        *sig = 1;
    }
    else
    {
        init += 2;
        while (*init == ' ' || *end == '\t')
            init ++;
        start = init;
    }
    return (start);
}

void    mini_built_in(t_cmd *cmd, t_env_var **g_env_list)
{
    //printf("mini_built_in\n\n");
	pid_t   p;
	//char	*argv[] = {NULL };
	char	*envp[] = { "PATH=/bin", "TERM=xterm" , NULL };


	cmd->arg = ft_split(cmd->root->command, ' ');
	if (!cmd)
		return ;
	if (ft_strcmp("cd", cmd->arg[0]) == 0)
		mini_cd(cmd->arg[1], *g_env_list);
	else if (ft_strcmp("pwd", cmd->arg[0]) == 0)
		mini_pwd();
	else if (ft_strcmp("exit", cmd->arg[0]) == 0)
		mini_exit();
	else if (ft_strcmp("export", cmd->arg[0]) == 0)
		mini_export(cmd->arg, g_env_list);
	else if (strcmp(cmd->arg[0], "env") == 0)
		mini_env(*g_env_list);
    else if (strcmp(cmd->arg[0], "unset") == 0)
		mini_unset(cmd->arg, g_env_list);
    else if (ft_strcmp("echo", cmd->arg[0]) == 0)
		mini_echo(cmd->line);
    /*else if ('.' == cmd->arg[0][0]) EXECUTAR UM EXECUTAVEL EX:./a.out
    {
        printf("%c\n", cmd->arg[0][0]);
        char	*argv[] = {"./minishell", NULL };
        char	*envp[] = { NULL };
        p = fork();
		if (p == 0)
        {
            execve(argv[0], argv, envp);
            printf("error\n");
        }
        waitpid(p, NULL, 0);
    }*/
	else
	{
		p = fork();
		if (p == 0)
        {
            char **execve_args = get_args(cmd->root->command);
            printf("nenhum mini_built_in\n\n");
            execve(execve_args[0], execve_args, envp);
            printf("error: ao executar o comando: %s\n", cmd->root->command);
        }
		waitpid(p, NULL, 0);
	}

	return ;
}

void	list_env_vars(t_env_var *g_env_list)
{
    t_env_var *current = g_env_list;
    while (current)
	{
        printf("%s=%s\n", current->name, current->value);
        current = current->next;
    }
}
