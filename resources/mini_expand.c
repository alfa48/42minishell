/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:07:45 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/06 11:24:10 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_matrix_safe(char **matrix)
{
	int 	i;

	i = -1;
	if (!matrix)
		return ;
	while (matrix[++i] != NULL)
	{
		if (matrix[i] != NULL)
			free(matrix[i]);
	}
	free(matrix);
}

char	*ft_strndup(const char *s, size_t n)
{
	size_t		len;
	size_t		i;
	char	*dup;

	len = 0;
	if (!s)
		return (NULL);
	while (s[len] && len < n)
		len ++;
	if (!(dup = (char *)malloc(len + 1)))
		return (NULL);
	i = -1;
	while (++i < len)
		dup[i] = s[i];
	dup[len] = '\0';
	return (dup);
}

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*new_str;

	if (!s1 || !s2)
		return (NULL);
	new_str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, s1, ft_strlen(s1) + 1);
	mini_strcat(new_str, s2);
	free(s1);
	return (new_str);
}

char    *expand_variable(char *str, char *result, int *i, t_env_var *g_env_list)
{
	int		var_start;
	char	*var_name;
	char	*var_value;
	(void)g_env_list;

	var_start = *i + 1;
	while (ft_isalnum(str[var_start]) || str[var_start] == '_')
		var_start ++;
	var_name = ft_strndup(str + *i + 1, var_start - (*i + 1));
	if (var_start++ == '?')
		var_value = cmd->status_cmd;
	else
		//var_value = ft_findenv(var_name, cmd->g_env_list);
	if (!var_value)
		//var_value = ft_findenv_fake(var_name, cmd->val_only;
	free(var_name);
	if (var_value)
		result = ft_strjoin_free(result, var_value);
	*i = var_start;
	return (result);
}

char	*process_char(char *str, char *r, int *i, bool *in_s_q, bool *in_d_q, t_env_var *env)
{
	char	temp[2];

	if (str[*i] == '\'' && !*in_d_q)
	{
		*in_s_q = !*in_s_q;
		r = ft_strjoin_free(r, "'");
		(*i) ++;
	}
	else if (str[*i] == '"' && !*in_s_q)
	{
		*in_d_q = !*in_d_q;
		r = ft_strjoin_free(r, "\"");
		(*i) ++;
	}
	else if (str[*i] == '$' && !*in_s_q &&
		(ft_isalnum(str[*i + 1]) || str[*i + 1] == '_' || str[*i + 1] == '?'))
		r = expand_variable(str, r, i, env);
	else
	{
		temp[0] = str[*i];
		temp[1] = '\0';
		r = ft_strjoin_free(r, temp);
		(*i)++;
	}
	return (r);
}

char	*expand_string(char *str, t_env_var *env_l)
{
	int		i;
	char	*result;
	bool	in_s_q;
	bool	in_d_q;

	i = 0;
	result = malloc(1);
	in_s_q = false, in_d_q = false;
	if (!result)
		return (NULL);
	result[0] = '\0';
	while (str[i])
	{
		result = process_char(str, result, &i, &in_s_q, &in_d_q, env_l);
		if (!result)
			return (NULL);
	}
 	return (result);
}

char	*expanding(char *str, t_env_var *g_env_list)
{
	if (!str || !g_env_list)
		return (NULL);
	if (!check_quotes_balance(str))
		return (NULL);
	return (expand_string(str, g_env_list));
}