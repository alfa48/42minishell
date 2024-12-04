/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:07:45 by fjilaias          #+#    #+#             */
/*   Updated: 2024/12/04 16:39:37 by fjilaias         ###   ########.fr       */
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

static char	**ft_sep_words(char **strs, int size, char *s)
{
	int	count;
	int	c_word;
	int	temp;

	count = 0;
	c_word = 0;
	while (c_word < size)
	{
		// Avança até encontrar uma palavra
		while (s[count] == ' ' && s[count] != '\0')
			count ++;
		temp = count;
		// Avança até o primeiro espaço (fim da palavra)
		while (s[temp] != ' ' && s[temp] != '\0')
			temp ++;
		// Adiciona tudo após o primeiro espaço à mesma string
		while (s[temp] == ' ' && s[temp] != '\0')
			temp ++;
		strs[c_word] = malloc((temp - count + 1) * sizeof(char));
		// Dar free na matriz se a alocacao falhar
		ft_memcpy(strs[c_word], &s[count], temp - count);
		strs[c_word][temp - count] = '\0';
		count = temp;
		c_word ++;
	}
	strs[c_word] = NULL;
	return (strs);
}

char	**ft_extra_split(char const *s)
{
	char	**strs;
	int		pos;
	int		size; 

	pos = 0;
	size = 0;
	if (s == NULL || ft_strlen(s) == 0)
		return (NULL);
	while (s[pos] != '\0')
	{
		if (s[pos] != ' ' && (s[pos + 1] == ' ' || s[pos + 1] == '\0'))
			size ++;
		pos ++;
	}
	strs = malloc((size + 1) * sizeof(char *));
	if (strs == NULL)
		return (NULL);
	strs = ft_sep_words(strs, size, (char *)s);
	return (strs);
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

char	*set_it_well(const char *str, const char *value)
{
	size_t	prefix_len;
	size_t	suffix_start;
	size_t	total_len;
	char	*out;

	prefix_len = 0;
	suffix_start = 0;
    // Localiza onde o "$" começa
	while (str[prefix_len] && str[prefix_len] != '$')
		prefix_len ++;
    // Localiza o final do nome da variável
	suffix_start = prefix_len + 1;
	while (str[suffix_start] && (ft_isalnum(str[suffix_start])
		|| str[suffix_start] == '_'))
        suffix_start ++;
    // Calcula o tamanho total: prefixo + valor expandido + sufixo
	total_len = prefix_len + ft_strlen(value) + ft_strlen(str + suffix_start) + 1;
	out = (char *)malloc(total_len);
	if (!out)
		return (NULL);
    // Copia o prefixo
	ft_strlcpy(out, str, prefix_len + 1);
    // Concatena o valor expandido
	ft_strlcat(out, value, total_len);
    // Concatena o sufixo (caracteres após a variável)
	ft_strlcat(out, str + suffix_start, total_len);
	return (out);
}

char	**expanding(char *str, t_env_var *g_env_list)
{
	char	**out;
	char	*tmp;
	char	*env;
	char	*p;
	int		i;

	i = -1;
	if (!(out = ft_extra_split(str)))
		return (NULL);
	while (out[++i] != NULL)
	{
		p = ft_strchr(out[i], '$');
		if ((p) && ft_isalnum(*(p + 1)) && *(p + 1) != '\0')
		{
			env = ft_findenv(p + 1, g_env_list);
			if (env)
			{
				tmp = out[i];
				out[i] = set_it_well(out[i], env);
				free(tmp);
				i --;
			}
			else
			{
				ft_memset_space(ft_strchr(out[i], '$'), ' ', ft_strlen(out[i]));
				i --;
			}
		}
	}
	return (out);
}