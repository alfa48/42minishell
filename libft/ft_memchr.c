/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:07:40 by manandre          #+#    #+#             */
/*   Updated: 2024/05/21 00:07:34 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *ptr, int value, size_t n)
{
	unsigned char	value_tmp;
	unsigned char	*ptr_tmp;

	ptr_tmp = (unsigned char *)ptr;
	value_tmp = (unsigned char)value;
	while (n--)
	{
		if (*ptr_tmp == value_tmp)
			return ((void *)ptr_tmp);
		ptr_tmp++;
	}
	return (NULL);
}
