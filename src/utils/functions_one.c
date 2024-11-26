/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions_one.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:15:30 by manandre          #+#    #+#             */
/*   Updated: 2024/10/23 12:11:25 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	if (!size)
		return (strlen(src));
	i = 0;
	while (i < size - 1 && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (strlen(src));
}


static char	*ft_strcpy(char *dest, const char *src)
{
	size_t i;

	i = -1;
	while (src[++i] != '\0')
		dest[i] = src[i];
	dest[i] = '\0';

	return dest;
}

char *ft_concat_char(char *str, char c)
{
	size_t	len;
	char *new_str;
	
	//len = ft_strlen(str);
	len = strlen(str);
	new_str = (char *)malloc((len + 2) * sizeof(char));
	if (new_str == NULL)
	{
		printf("Erro ao alocar memória\n");
		return NULL;
        }
        ft_strcpy(new_str, str);
        //strncpy(new_str, str, len + 1);
        new_str[len] = c;
        new_str[len + 1] = '\0';
	free(str);
        return (new_str);
}


char	*rm_last_char(char *str)
{
    size_t	len;
    
     //len = ft_strlen(str);
     len = strlen(str);
    if (len == 0)
        return strdup("");
    char *new_str = (char *)malloc(len * sizeof(char));
    if (new_str == NULL)
    {
        printf("Erro ao alocar memória\n");
        return NULL;
    }

    ft_strlcpy(new_str, str, len);
    //strncpy(new_str, str, len);
    new_str[len - 1] = '\0';
    free(str);
    return new_str;
}

