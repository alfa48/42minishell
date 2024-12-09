/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cepeu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:04:22 by fjilaias          #+#    #+#             */
/*   Updated: 2024/12/09 09:47:58 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_quotes_balance(const char *arg)
{
	int	single_quotes;
	int	double_quotes;

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
		return (true);
	else
	{
		printf("Error: unclosed quotes\n");
		return (false);
	}
}

bool	is_valid_var_char(char c)
{
    return (ft_isalnum(c) || c == '_');
}

char	*ft_findenv(char *s, t_env_var *g_env_list)
{
	size_t	len;
	t_env_var	*tmp;

	tmp = g_env_list;
	while (tmp)
	{
		len = ft_strlen(tmp->name);
		if (ft_strncmp(s, tmp->name, len) == 0)
		{
			if (!is_valid_var_char(s[len]))
				return (tmp->value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

static int	process_quote_char(char *init, bool *in_s_q, bool *in_d_q)
{
    if (*init == '\'' && !*in_d_q)
    {
        *in_s_q = !*in_s_q;
        return (1);
    }
    else if (*init == '"' && !*in_s_q)
    {
        *in_d_q = !*in_d_q;
        return (1);
    }
    return (0);
}

void	mini_echo(t_env_var *env_var, char *arg)
{
    bool	in_s_q;
    bool	in_d_q;
    int		new_line;
    char	*init;

    new_line = 0;
    if (!check_quotes_balance(arg))
        return ;
    arg = expanding(arg, env_var);
    in_s_q = false;
    in_d_q = false;
    init = get_word(arg, &new_line);
    if (!init)
        return;
    while (*init)
    {
        while (*init == ' ' && !in_s_q && !in_d_q)
        {
            init ++;
            if (*init && *init != ' ')
				write(1, " ", 1);
        }
        if (!process_quote_char(init, &in_s_q, &in_d_q))
			write(1, init, 1);
		if (*init)
			init ++;
    }
    free(arg);
    if (new_line)
        write(1, "\n", 1);
}

void	mini_cd(char *path, t_env_var *g_env_list)
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

void	mini_pwd(void)
{
	char    cwd[42000];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("pwd");
}

void	mini_export(char **args, t_env_var **g_env_list)
{
	int	i;
	
	if (!args[1])
	{
		mini_env(*g_env_list);
		return ;
	}
	i = 1;
	set_or_add_env_var(args[i], g_env_list);
}

void	mini_unset(char **args, t_env_var **g_env_list)
{
	t_env_var	*current;
	t_env_var	*tmp;

	if (args == NULL)
		return ;
	current = *g_env_list;
	if (strcmp(args[1], current->name) == 0)
	{
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
