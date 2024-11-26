/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 11:55:38 by manandre          #+#    #+#             */
/*   Updated: 2024/05/21 00:09:32 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	x;
	size_t	y;
	size_t	z;

	if (*needle == '\0')
		return ((char *)haystack);
	x = 0;
	while (haystack[x] && x < len)
	{
		y = 0;
		z = x;
		while (haystack[z] == needle[y] && needle[y] && z < len)
		{
			y++;
			z++;
		}
		if (!needle[y])
			return ((char *)&haystack[x]);
		x++;
	}
	return (0);
}
