/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_execv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:56:05 by fjilaias          #+#    #+#             */
/*   Updated: 2024/12/04 07:43:04 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *get_word(char *line, int *sig)
{
	char    *init;
	char    *end;
	char    *start;
	int     i;

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
		mini_echo(*g_env_list, cmd->line);
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

static int compare_env_vars(const void *a, const void *b)
{
	t_env_var *varA = *(t_env_var **)a;
	t_env_var *varB = *(t_env_var **)b;
	return (strcmp(varA->name, varB->name));
}  

void	only_expor_cmd(t_env_var *g_env_list)
{  
    // Contar o número de variáveis de ambiente  
    int count = 0;  
    t_env_var *current = g_env_list;  
    while (current) {  
        count++;  
        current = current->next;  
    }  
    // Criar um array para armazenar os ponteiros para as variáveis  
    t_env_var **env_array = malloc(count * sizeof(t_env_var *));  
    if (!env_array) {  
        perror("malloc");  
        return ; 
    }  
    // Preencher o array com os ponteiros para as variáveis  
    current = g_env_list;  
    for (int i = 0; i < count; i++)
    {  
        env_array[i] = current;  
        current = current->next;  
    }  
    // Ordenar o array  
    qsort(env_array, count, sizeof(t_env_var *), compare_env_vars);  
    // Imprimir as variáveis no formato desejado  
    for (int i = 0; i < count; i++) {  
        printf("declare -x %s=\"%s\"\n", env_array[i]->name, env_array[i]->value);  
    }  
    // Liberar a memória alocada  
    free(env_array);  
}
