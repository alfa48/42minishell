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


char *ft_sub(char *ptr_start, char *str2)
{
    if (!ptr_start || !str2)
        return (NULL);

    char *pos = ptr_start; 
    char *end = pos;

   while (*end > 32)
        end++;

    size_t prefix_len = pos - ptr_start;
    size_t new_len = prefix_len + strlen(str2) + strlen(end) + 1;

    char *new = malloc(new_len);
    if (!new)
        return (NULL);

    strncpy(new, ptr_start, prefix_len);
    strcpy(new + prefix_len, str2);
    strcat(new, end);

    strcpy(ptr_start, new);
    free(new);
    return (ptr_start);
}


char *ft_findenv(char *s, t_env_var *g_env_list)
{
    int i = 0;

    while (s[i] > 32)
        i++;

    t_env_var *tmp_env_var = g_env_list;
    while (tmp_env_var)
    {
        int len = strlen(tmp_env_var->name);
        if (strncmp(tmp_env_var->name, s + 1, i - 1) == 0 && len == (i - 1))
            return (tmp_env_var->value);
        tmp_env_var = tmp_env_var->next;
    }
    return (NULL);
}

char *mini_expand(t_env_var *g_env_var, char *str)
{
    char *tmp = str;
    char *var_env;

   while (*tmp && *tmp != '$')
        tmp++;

    if (*tmp == '$')
    {
        var_env = ft_findenv(tmp, g_env_var);
        if (!var_env)
            ft_sub(tmp, " ");
        else
            ft_sub(tmp, var_env);
    }
    return (str);
}

char *is_expand(t_env_var *env_var, char *s)
{
    char *tmp = s;

    while ((tmp = strchr(tmp, '$')) != NULL)
    {
        if (tmp[1] > 32)
        {
            s = mini_expand(env_var, tmp);
            env_var->counter_exp++;
        }
        tmp++;
    }
    return (s);
}

void    mini_echo(t_env_var *env_var, char *arg)
{
    bool    inside_single_quote;
    bool    inside_double_quote;
    int     new_line;
    char    *init;

    (void) env_var;
   
        is_expand(env_var, arg);
    printf("STRING PARA O ECHO: %s\n", arg);
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