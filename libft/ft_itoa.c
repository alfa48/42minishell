/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 06:47:30 by manandre          #+#    #+#             */
/*   Updated: 2024/05/21 07:34:23 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_char(long int number)
{
	size_t	size;

	size = 0;
	if (number == 0)
		return (1);
	if (number < 0)
	{
		number = number * -1;
		size++;
	}
	while (number > 0)
	{
		number = number / 10;
		size++;
	}
	return (size);
}

static void	ft_convbase(long int n, char *number, long int i)
{
	if (n < 0)
	{
		number[0] = '-';
		n *= -1;
	}	
	if (n >= 10)
		ft_convbase((n / 10), number, (i - 1));
	number[i] = (n % 10) + '0';
}

char	*ft_itoa(int n)
{
	size_t	size;
	char	*str;

	size = count_char(n);
	str = malloc(sizeof(char) * (size + 1));
	if (!str)
		return (0);
	str[size--] = '\0';
	ft_convbase(n, str, size);
	return (str);
}
