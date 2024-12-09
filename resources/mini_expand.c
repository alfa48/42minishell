/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:07:45 by fjilaias          #+#    #+#             */
/*   Updated: 2024/12/09 11:46:58 by fjilaias         ###   ########.fr       */
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

/*char **ft_extra_split(const char *s)
{
    if (!s || !*s) return NULL;

    char **strs = NULL;
    int size = 0, count = 0, in_quote = 0;
    char quote_char = '\0';

    for (int i = 0; s[i]; i ++) {
        if ((s[i] == '\'' || s[i] == '"') && (!in_quote || s[i] == quote_char)) {
            in_quote = !in_quote;
            quote_char = in_quote ? s[i] : '\0';
        } else if (!in_quote && s[i] == ' ') {
            size++;
        }
    }

    strs = malloc((size + 2) * sizeof(char *));
    if (!strs) return NULL;

    in_quote = 0;
    quote_char = '\0';
    const char *start = s;

    for (int i = 0; s[i]; i++) {
        if ((s[i] == '\'' || s[i] == '"') && (!in_quote || s[i] == quote_char)) {
            in_quote = !in_quote;
            quote_char = in_quote ? s[i] : '\0';
        } else if (!in_quote && s[i] == ' ') {
            strs[count++] = strndup(start, s + i - start);
            start = s + i + 1;
        }
    }

    if (*start) {
        strs[count++] = strdup(start);
    }

    strs[count] = NULL;
    return strs;
}

char	*concat_strings(char **str_array)
{
	char	*result;
	size_t		total_length;
	int		i;
	int		j;

	i = 0;
	total_length = 0;
	while (str_array[i])
	{
		total_length += ft_strlen(str_array[i]);
		i ++;
	}
	total_length += (i - 1);
	result = malloc((total_length + 1) * sizeof(char));
	if (!result)
	{
		perror("Not enough memory");
		exit(1);
	}
	result[0] = '\0';
	j = -1;
	while (str_array[++j])
		mini_strcat(result, str_array[j]);
	free_matrix_safe(str_array);
	return (result);
}

// Função auxiliar para verificar se estamos dentro de aspas
bool is_inside_quotes(const char *str, int pos, char quote_type) {
    bool inside = false;
    for (int i = 0; i < pos; i++) {
        if (str[i] == quote_type) {
            inside = !inside;
        }
    }
    return inside;
}

// Função para processar uma única variável e expandi-la
char *expand_variable(const char *str, int var_start, t_env_var *g_env_list) {
    int var_end = var_start + 1;
    while (isalnum(str[var_end]) || str[var_end] == '_') {
        var_end++;
    }
    char *var_name = strndup(str + var_start + 1, var_end - var_start - 1);
    char *var_value = ft_findenv(var_name, g_env_list);
    free(var_name);
    if (!var_value) {
        var_value = ""; // Substituir por vazio se não encontrar
    }

    // Construir nova string com a expansão
    size_t new_len = strlen(str) - (var_end - var_start) + strlen(var_value) + 1;
    char *new_str = malloc(new_len);
    if (!new_str) return NULL;

    strncpy(new_str, str, var_start);       // Prefixo
    strcpy(new_str + var_start, var_value); // Valor expandido
    strcat(new_str, str + var_end);         // Sufixo
    return new_str;
}*/

char	*ft_strndup(const char *s, size_t n)
{
	size_t	len;
	size_t	i;
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

bool	is_valid_var_start(char c)
{
	return (ft_isalnum(c) || c == '_');
}

void	handle_regular_char(char *str, int *i, char **result)
{
	char temp[2];

    temp[0] = str[*i];
	temp[1] = '\0';
    *result = ft_strjoin_free(*result, temp);
    (*i) ++;
}

void	handle_variable(char *str, int *i, char **result, t_env_var *g_env_list)
{
	int		var_start;
	char	*var_name;
	char	*var_value;

    var_start = *i + 1;
    while (is_valid_var_start(str[var_start]))
        var_start ++;
    var_name = ft_strndup(str + *i + 1, var_start - (*i + 1));
    var_value = ft_findenv(var_name, g_env_list);
    free(var_name);
    if (var_value)
        *result = ft_strjoin_free(*result, var_value);
    *i = var_start;
}


void	handle_quotes(char *str, int *i, char **result, bool *in_s_q, bool *in_d_q)
{
    if (str[*i] == '\'' && !(*in_d_q))
	{
        *in_s_q = !(*in_s_q);
        *result = ft_strjoin_free(*result, "'");
    }
	else if (str[*i] == '"' && !(*in_s_q))
	{
        *in_d_q = !(*in_d_q);
        *result = ft_strjoin_free(*result, "\"");
    }
    (*i)++;
}

char	*expanding(char *str, t_env_var *g_env_list)
{
	bool	in_s_q;
	bool	in_d_q;
	char	*result;
	int		i;

	i = 0;
    in_s_q = false, in_d_q = false;
    if (!str || !g_env_list)
        return (NULL);
    result = malloc(1);
    if (!result)
        return (NULL);
    result[0] = '\0';
    while (str[i])
	{
        if (str[i] == '\'' || str[i] == '"')
            handle_quotes(str, &i, &result, &in_s_q, &in_d_q);
        else if (str[i] == '$' && !in_s_q && is_valid_var_start(str[i + 1]))
            handle_variable(str, &i, &result, g_env_list);
        else
            handle_regular_char(str, &i, &result);
    }
    return (result);
}