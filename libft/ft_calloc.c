/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 22:44:13 by manandre          #+#    #+#             */
/*   Updated: 2024/05/23 12:56:09 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t number, size_t size)
{
	void	*ptr;

	if (number != 0 && size != 0 && (number > 4294967295 / size))
		return (0);
	ptr = malloc(number * size);
	if (!ptr)
		return (ptr);
	ft_bzero(ptr, number * size);
	return (ptr);
}
