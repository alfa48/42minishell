/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cepeu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:04:22 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/06 13:23:29 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_quotes_balance(const char *arg)
{
	bool	in_single_quote;
	bool	in_double_quote;

	in_single_quote = false;
	in_double_quote = false;
    while (*arg)
	{
        if (*arg == '\'' && !in_double_quote)
            in_single_quote = !in_single_quote;
        else if (*arg == '"' && !in_single_quote)
            in_double_quote = !in_double_quote;
        arg ++;
    }
    if (!in_single_quote && !in_double_quote)
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

char	*process_cmd(char *arg)
{
	char	*out;
    bool	in_s_q;
    bool	in_d_q;

    in_s_q = false;
    in_d_q = false;
	if (!(out = malloc((ft_strlen(arg) + 2))))
		return NULL;
	char *tmp = out;
    while (*arg)
    {
        while (*arg == ' ' && !in_s_q && !in_d_q)
        {
            arg ++;
            if (*arg && *arg != ' ')
			{
				*out = ' ';
				out ++;
			}
        }
        if (!process_quote_char(arg, &in_s_q, &in_d_q))
		{
				*out = *arg;
				out ++;
		}
		if (*arg)
			arg ++;
    }
     *out = '\0';
	return (tmp);
}

void	mini_echo(char *arg)
{
    int		new_line;
	char	*init;
	char	*print;

    new_line = 0;
    if (!(init = get_word(arg, &new_line)))
        return ;
	else
	{
		print = process_cmd(init);
		write(1, print, ft_strlen(print));
	}
	free(print);
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
	int		i;
	
	if (!args[1])
	{
		only_expor_cmd(*g_env_list);
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
