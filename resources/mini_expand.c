/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:07:45 by fjilaias          #+#    #+#             */
/*   Updated: 2024/12/03 09:42:32 by fjilaias         ###   ########.fr       */
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
	int		i;
	int		j;
	int		no_space;
	int		space;
	char	*out;
    
	i = 0;  
	no_space = 0;  
	space = 0;
	while (str[i])
	{  
		if (str[i] > 32)
  		      	no_space ++;  
		else if (str[i] == ' ')
			space ++;
		i ++;
	}
	out = (char *)malloc(sizeof(char) * (ft_strlen(value) + space + 1));
	if (!out)
		return NULL;
	ft_strlcpy(out, value, ft_strlen(value) + 1);
	j = ft_strlen(value);
	i = 0;  
	while (str[i] != '\0')
	{  
		if (str[i] == ' ')
			out[j++] = ' ';
		i ++;
	}
	out[j] = '\0';
	return (out);  
}

char	**expanding(char *str, t_env_var *g_env_list)
{
	char	**out;
	char	*tmp;
	char	*env;
	char	*var_name;
	int		i;

	out = ft_extra_split(str);
	if (out == NULL)
	    return (NULL);
	i = 0;
	while (out[i] != NULL)
	{
		if (out[i][0] == '$')
		{
		    var_name = mini_epur_str(out[i] + 1);
		    env = ft_findenv(var_name, g_env_list);
		    if (env != NULL)
		    {
		        tmp = out[i];
				out[i] = set_it_well(out[i], env);
				free(tmp);
			}
			else
			{
			    free(out[i]);
			    out[i] = ft_strdup("");
			}
			free(var_name);
		}
		i ++;
	}
	return (out);
}