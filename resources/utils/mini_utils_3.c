/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:13:10 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/30 23:40:49 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_it_repeated(char *name, const char *value, t_env_var *current)
{
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			if (!current->value)
				return (0 * printf("Failed to allocate memory\n"));
			return (42);
		}
		current = current->next;
	}
	return (0);
}

int	set_it(char *name, const char *value, t_env_var **g_env_list)
{
	t_env_var	*current;
	t_env_var	*new_var;

	current = *g_env_list;
	if (is_it_repeated(name, value, current) == 42)
		return (42);
	new_var = malloc(sizeof(t_env_var));
	if (!new_var)
		return (0);
	new_var->name = ft_strdup(name);
	if (ft_strlen(value) > 2097152)
	{
		free(new_var);
		ft_putstr_fd("Too long env value\n", 2);
		return (0);
	}
	if (value)
		new_var->value = ft_strdup(value);
	else
		new_var->value = ft_strdup("");
	if (!new_var->name || !new_var->value)
		return (0);
	new_var->next = *g_env_list;
	*g_env_list = new_var;
	return (1);
}

void	set_or_add_env_var(const char *env_entry, t_env_var **g_env_list)
{
	char		name[256];
	char		*equal_sign;
	const char	*value;
	size_t		name_len;

	equal_sign = ft_strchr(env_entry, '=');
	if (equal_sign)
	{
		name_len = (equal_sign - env_entry) + 1;
		ft_strlcpy(name, env_entry, name_len);
		name[name_len] = '\0';
		value = equal_sign + 1;
		if (!is_valid_env_var_name(name))
			return ;
		if (set_it(name, value, g_env_list) == 42)
			return ;
	}
	else
	{
		name_len = ft_strlen(env_entry) + 1;
		ft_strlcpy(name, env_entry, name_len);
		name[name_len] = '\0';
		if (set_it(name, "", g_env_list) == 42)
			return ;
	}
}

void	initialize_env_list(t_env_var **g_env_list, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		set_or_add_env_var(envp[i], g_env_list);
		i++;
	}
}

char	*mini_strstr(const char *haystack, const char *needle)
{
	const char	*h;
	const char	*n;

	if (!*needle)
		return ((char *)haystack);
	while (*haystack)
	{
		h = haystack;
		n = needle;
		while (*h && *n && *h == *n)
		{
			h++;
			n++;
		}
		if (!*n)
			return ((char *)haystack);
		haystack++;
	}
	return (NULL);
}
