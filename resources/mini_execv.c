/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_execv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:56:05 by fjilaias          #+#    #+#             */
/*   Updated: 2024/12/13 13:32:54 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
  
char*	my_strtok(char* str, const char* delimiters)
{  
	static char* next_token = NULL; // Ponteiro estático para a próxima posição a ser analisada na string  

    // Se a string for nula, começamos a nova divisão da última string processada  
    if (str != NULL)
        next_token = str; // Inicializa com a nova string  

    // Se não há mais tokens para processar, retornamos NULL  
    if (next_token == NULL)  
        return NULL;  

    // Pula os delimitadores iniciais  
    char* token_start = next_token; // Marca o início do token  
    while (*next_token && ft_strchr(delimiters, *next_token) != NULL) 
        next_token++; // Avança até o primeiro caractere não delimitador  

    // Se atingirmos o final da string, não há mais tokens  
    if (*next_token == '\0')  
        return NULL;  

    // Avança até o final do token  
    while (*next_token && ft_strchr(delimiters, *next_token) == NULL)
        next_token++; // Continua até encontrar um delimitador  

    // Se encontramos um delimitador, substituímos por \0 e ajustamos o ponteiro   
    if (*next_token) {  
        *next_token = '\0'; // Terminamos o token  
        next_token++; // Avança para o próximo caractere para a próxima chamada  
    }
    return token_start; // Retorna o início do token encontrado  
}  

void	mini_val(char *str, t_env_var **g_env_list)
{
	char	*name;
	char	*token;
	char	*value;
	char	*equals_sign;
	int		i =0;
	char *ntk = NULL;

    token = my_strtok(str, " \t");
	str = token;
    while (token)
	{
        // Verifique se o token está no formato "VAR=VAL"
        equals_sign = ft_strchr(token, '=');
        if (equals_sign)
		{
            *equals_sign = '\0';
            name = token;
            value = equals_sign + 1;
            set_it(name, value, g_env_list);
        }
		else if (!i)
		{	
			ntk = ft_strdup(token);
			i ++;
		}
		name = ft_strdup("");
		value = ft_strdup("");
		token = my_strtok(NULL, " \t");
    }
	if (i > 0)
	{
		printf("apagar a lista. nao criar variaveis e executar o comando com o: %s\n", ntk);
	}

	free(ntk);
}

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
	else
	{
		p = fork();
		if (p == 0)
		{
		    char **execve_args = get_args(cmd->root->command);
		    mini_val(cmd->line, &(cmd->val_only));
		    list_env_vars(cmd->val_only);
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
