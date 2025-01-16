/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 13:22:09 by manandre          #+#    #+#             */
/*   Updated: 2025/01/16 13:22:10 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *ft_strjoin_gnl(char *s1, char *s2)
{
    char    *str;
    int     i;
    int     j;

    if (!s1)
    {
        s1 = malloc(sizeof(char) * 1);
        if (!s1)
            return (NULL);
        s1[0] = '\0';
    }
    if (!s2)
        return (NULL);
    str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
    if (!str)
        return (NULL);
    i = -1;
    j = 0;
    while (s1[++i])
        str[i] = s1[i];
    while (s2[j])
        str[i++] = s2[j++];
    str[i] = '\0';
    free(s1);
    return (str);
}

static char *get_line(char *saved)
{
    int     i;
    char    *str;

    i = 0;
    if (!saved[i])
        return (NULL);
    while (saved[i] && saved[i] != '\n')
        i++;
    str = malloc(sizeof(char) * (i + 2));
    if (!str)
        return (NULL);
    i = 0;
    while (saved[i] && saved[i] != '\n')
    {
        str[i] = saved[i];
        i++;
    }
    if (saved[i] == '\n')
    {
        str[i] = saved[i];
        i++;
    }
    str[i] = '\0';
    return (str);
}

static char *save_next(char *saved)
{
    int     i;
    int     j;
    char    *str;

    i = 0;
    while (saved[i] && saved[i] != '\n')
        i++;
    if (!saved[i])
    {
        free(saved);
        return (NULL);
    }
    str = malloc(sizeof(char) * (ft_strlen(saved) - i + 1));
    if (!str)
        return (NULL);
    i++;
    j = 0;
    while (saved[i])
        str[j++] = saved[i++];
    str[j] = '\0';
    free(saved);
    return (str);
}

static char *read_file(int fd, char *saved)
{
    char    *buffer;
    int     bytes_read;

    buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
    if (!buffer)
        return (NULL);
    bytes_read = 1;
    while (bytes_read != 0 && !ft_strchr(saved, '\n'))
    {
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (bytes_read == -1)
        {
            free(buffer);
            free(saved);
            return (NULL);
        }
        buffer[bytes_read] = '\0';
        saved = ft_strjoin_gnl(saved, buffer);
    }
    free(buffer);
    return (saved);
}

char *get_next_line(int fd)
{
    char        *line;
    static char *saved;

    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);
    saved = read_file(fd, saved);
    if (!saved)
        return (NULL);
    line = get_line(saved);
    saved = save_next(saved);
    return (line);
}