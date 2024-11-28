/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cepeu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:04:22 by fjilaias          #+#    #+#             */
/*   Updated: 2024/11/27 15:39:33 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool check_quotes_balance(const char *arg)
{
    int single_quotes;
    int double_quotes;

    single_quotes = 0;
    double_quotes = 0;
    while (*arg)
    {
        if (*arg == '\'')
            single_quotes ++;
        else if (*arg == '"')
            double_quotes ++;
        arg ++;
    }
    if (single_quotes % 2 == 0 && double_quotes % 2 == 0)
    {
        if (ft_strchr(arg, '"') > ft_strchr(arg, '\''))

        return (true);
    }
    return (false);
}


static char *ft_sub(char *str1, char *ptr_start, char *str2)
{

    if (!ptr_start || !str1 || !str2) 
        return (NULL);

    printf("str1: %s, start: %c, sub: %s\n", str1, ptr_start[0], str2);
    
    char *pos = ptr_start; 
    char *end = pos;
    printf("Esta é o ft_sub\n");
    while (*end <= 32)
        end++;  
    size_t newlen = strlen(str1) - (end - pos) + strlen(str2);
    char new[newlen + 1];
    char *p = new; 
    strncpy(p, str1, pos - str1);  
    p += pos - str1;  
    strcpy(p, str2);  
    p += strlen(str2);  
    strcpy(p, end);   
    strcpy(str1, new); 
    printf("Esta é o ft_sub fim\n");
    return (str1);
}

char    *ft_findenv(char *s, t_env_var *g_env_list)
{
    int i;
    t_env_var *tmp;

    i = 0;
    printf("Esta é o ft_findenv\n");
    while (s[i] && s[i] != ' ')
        i ++;
    tmp = g_env_list;
    while (tmp)
    {
        //printf("inicio %c e fim %c\n", s[0], s[i - 1]);
        int len = (int)ft_strlen(tmp->name);
        printf("v = %i e v2 = %i\n", len, i -1);
            // == (i - 1))
        if ((ft_strncmp(tmp->name, s + 1, i - 1) == 0) && (len == (i - 1)))
        {
            printf("Valor %s, Name %s\n", tmp->value, tmp->name);
            return (tmp->value);
        }
        tmp = tmp->next;
    }
   //  printf("Esta é o ft_findenv r: null\n");
    return (NULL);
}

char    *mini_expand(t_env_var *g_env_var, char *s)
{
    char    *start;
    char    *tmp;
    char *var_env;

    tmp = s;
    printf("Esta é o mini_expand\n");
    while (*s && *s != '$')
        s ++;
    if(*s == '$')
    {
        start = s;
       // printf("%s \n",  ft_findenv(start, g_env_var));
       var_env = ft_findenv(start, g_env_var);
        if (!var_env)
            ft_sub(s, start, " ");
        else
            ft_sub(s, start, var_env);
    }
    return (tmp);
}

int is_expand(t_env_var *env_var, char *s)
{
    int i = 0;
    char *tmp = s;
    
    while (tmp && (tmp = ft_strchr(tmp, '$')) != NULL)
    {
        if (tmp[1] > 32)
        {
            mini_expand(env_var, tmp);
            printf("%d Este é o TMP - %s \n", i, tmp);
            env_var->counter_exp++;
        }
        tmp++;
        i++;
    }
    return env_var->counter_exp;
}

void    mini_echo(t_env_var *env_var, char *arg)
{
    bool    inside_single_quote;
    bool    inside_double_quote;
    int     new_line;
    char    *init;

    is_expand(env_var, arg);
    inside_single_quote = false;
    inside_double_quote = false;
   /* if (!check_quotes_balance(arg))
    {
        write(2, "Error: unclosed quotes\n", 24);
        return ;
    }*/
    init = get_word(arg, &new_line);
    if (!init)
        return ;
    while (*init)
    {
        while (*init == ' ' && !inside_single_quote && !inside_double_quote)
        {
            init++;
            if (*init && *init != ' ')
                write(1, " ", 1);
        }

        if (*init == '\'' && !inside_double_quote)
            inside_single_quote = !inside_single_quote;
        else if (*init == '"' && !inside_single_quote)
            inside_double_quote = !inside_double_quote;
        else
            write(1, init, 1);
        init++;
    }
    if (new_line)
        write(1, "\n", 1);
}

void    mini_cd(char *path, t_env_var *g_env_list)
{
    char cwd[PATH_MAX];
    if (chdir(path) != 0)
    {
        perror("cd");
        return ;
    }
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        set_or_add_env_var(ft_strjoin("PWD=", cwd), &g_env_list);
    else
        perror("getcwd");
}

void    mini_pwd(void)
{
    char    cwd[42000];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s\n", cwd);
    else
        perror("pwd");
}

void    mini_export(char **args, t_env_var **g_env_list)
{
    int i;

    if (!args[1])
    {
        mini_env(*g_env_list);
        return ;
    }
    i = 1;
    set_or_add_env_var(args[i], g_env_list);
}

void    mini_unset(char **args, t_env_var **g_env_list)
{
    t_env_var   *current;
    t_env_var   *tmp;

    if (args == NULL)
        return ;
    current = *g_env_list;
    if (strcmp(args[1], current->name) == 0)
    {
        printf("A ser removido - %s\n", current->name);
        (*g_env_list) = current->next;
        current = NULL;
        return ;
    }
    while (current->next)
    {
        if (strcmp(args[1], current->next->name) == 0)
        {
            tmp =  current->next;
            current->next = current->next->next;
            free(tmp);
            return ;
        }
        current = current->next;
    }
}