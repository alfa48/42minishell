/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cepeu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:04:22 by fjilaias          #+#    #+#             */
/*   Updated: 2024/12/03 09:11:04 by fjilaias         ###   ########.fr       */
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
	return (false);
}

char	*ft_findenv(char *s, t_env_var *g_env_list)
{
	t_env_var *tmp;

	tmp = g_env_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->name, s) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

void	mini_echo(t_env_var *env_var, char *arg)
{
	bool    inside_single_quote;
	bool    inside_double_quote;
	int     	new_line;
	char    *init;

	new_line = 0;
	arg = concat_strings(expanding(arg, env_var));
	inside_single_quote = false;
	inside_double_quote = false;
	init = get_word(arg, &new_line);
	if (!init)
		return ;
	while (*init)
	{
		while (*init == ' ' && !inside_single_quote && !inside_double_quote)
		{
			init ++;
			if (*init && *init != ' ')
				write(1, " ", 1);
		}
		if (*init == '\'' && !inside_double_quote)
			inside_single_quote = !inside_single_quote;
		else if (*init == '"' && !inside_single_quote)
			inside_double_quote = !inside_double_quote;
		else
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
