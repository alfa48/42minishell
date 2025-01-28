/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:07:45 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/28 15:39:32 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*new_str;

	if (!s1 || !s2)
	{
		if (s1)
			free(s1);
		return (NULL);
	}
	new_str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!new_str)
	{
		free(s1);
		return (NULL);
	}
	ft_strlcpy(new_str, s1, ft_strlen(s1) + 1);
	mini_strcat(new_str, s2);
	free(s1);
	return (new_str);
}

char	*expand_variable(char *str, char *result, int *i, t_cmd *cmd)
{
	int		var_start;
	char	*var_name;
	char	*var_value;

	var_start = *i + 1;
	while (ft_isalnum(str[var_start]) || str[var_start] == '_')
		var_start++;
	var_name = ft_strndup(str + *i + 1, var_start - (*i + 1));
	var_value = ft_findenv(var_name, cmd->g_env_list);
	if (str[var_start] == '?')
	{
		var_start++;
		if (var_value)
			free(var_value);
		var_value = ft_itoa(cmd->status_cmd);
	}
	free(var_name);
	if (var_value)
	{
		result = ft_strjoin_free(result, var_value);
		free(var_value);
	}
	*i = var_start;
	return (result);
}

char	*process_char(char *str, char *r, int *i, t_cmd *cmd)
{
	char	temp[2];

	if (str[*i] == '\'' && !(cmd->in_d_q))
	{
		cmd->in_s_q = !cmd->in_s_q;
		r = ft_strjoin_free(r, "\'");
		(*i)++;
	}
	else if (str[*i] == '"' && !(cmd->in_s_q))
	{
		cmd->in_d_q = !cmd->in_d_q;
		r = ft_strjoin_free(r, "\"");
		(*i)++;
	}
	else if (str[*i] == '$' && !(cmd->in_s_q) && (ft_isalnum(str[*i + 1])
			|| str[*i + 1] == '_' || str[*i + 1] == '?'))
		r = expand_variable(str, r, i, cmd);
	else
	{
		temp[0] = str[*i];
		temp[1] = '\0';
		r = ft_strjoin_free(r, temp);
		(*i)++;
	}
	return (r);
}

char	*expand_string(char *str, t_cmd *cmd)
{
	int		i;
	char	*result;

	i = 0;
	cmd->in_s_q = false;
	cmd->in_d_q = false;
	result = ft_strdup("");
	if (!result)
	{
		free(str);
		return (NULL);
	}
	result[0] = '\0';
	while (str[i])
	{
		result = process_char(str, result, &i, cmd);
		if (!result)
		{
			free(str);
			return (NULL);
		}
	}
	return (result);
}

char	*expanding(char *str, t_cmd *cmd)
{
	char	*tmp;

	if (!str || !cmd->g_env_list)
		return (NULL);
	if (!check_quotes_balance(str))
		return (NULL);
	tmp = expand_string(str, cmd);
	free(str);
	if (!tmp)
		return (NULL);
	return (tmp);
}
