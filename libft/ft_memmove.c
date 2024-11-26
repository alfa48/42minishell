/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:00:02 by manandre          #+#    #+#             */
/*   Updated: 2024/05/21 08:09:01 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*dest_temp;
	char	*src_temp;
	size_t	i;

	dest_temp = (char *) dest;
	src_temp = (char *) src;
	i = n;
	if (dest_temp < src_temp)
	{
		return (ft_memcpy(dest, src, n));
	}
	else
	{
		while (i > 0)
		{
			i--;
			dest_temp[i] = src_temp[i];
		}
	}
	return (dest);
}
