/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:39:25 by manandre          #+#    #+#             */
/*   Updated: 2024/05/21 08:13:18 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *ptr1, const void *ptr2, size_t n)
{
	const unsigned char	*p1_temp;
	const unsigned char	*p2_temp;
	size_t				i;

	i = 0;
	p1_temp = (const unsigned char *)ptr1;
	p2_temp = (const unsigned char *)ptr2;
	while (i < n)
	{
		if (p1_temp[i] != p2_temp[i])
		{
			return (p1_temp[i] - p2_temp[i]);
		}
		i++;
	}
	return (0);
}
