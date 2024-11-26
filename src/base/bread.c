/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bread.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 14:55:49 by manandre          #+#    #+#             */
/*   Updated: 2024/10/23 14:55:51 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_fread(void);
char	gnc(int fd);

char	gnc(int fd)
{
	char	c;
	int	rs;

	if (fd < 0)
		return ('\0');
	rs = read(fd, &c, 1);
	if (rs == -1)
	{
		//printf("Erro ao ler o arquivo: %s\n", strerror(errno));
		return ('\0');
	} else if (rs == 0)
	{
		//printf("Fim do arquivo.\n");
		return '\0';
	}

    return c;
}

int	open_fread(void)
{
	int	fd;

	fd = open("files/out.txt", FILE_MODE_READ, 0644);
	if (fd < 0)
		return (-1);
	return (fd);
}
