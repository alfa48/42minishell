/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 21:35:26 by manandre          #+#    #+#             */
/*   Updated: 2024/05/20 23:27:47 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isspace(char c)
{
	if (c == ' ' || (c >= '\t' && c <= '\r'))
	{
		return (1);
	}
	return (0);
}

static int	ft_islong(long int sum, int signo)
{
	if (sum * signo > 2147483647)
		return (-1);
	else if (sum * signo < -2147483648)
		return (0);
	return (1);
}

int	ft_atoi(const char *str1)
{
	int			i;
	int			signo;
	long int	nb;
	char		*str;

	str = (char *)str1;
	nb = 0;
	i = 0;
	signo = 1;
	while (ft_isspace(str[i]) == 1)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			signo = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9' && str[i] != '\0')
	{
		if (ft_islong(nb, signo) == -1 || ft_islong(nb, signo) == 0)
			return (ft_islong(nb, signo));
		nb = nb * 10 + str[i] - '0';
		i++;
	}
	return (nb * signo);
}
