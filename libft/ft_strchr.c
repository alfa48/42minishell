/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 12:39:46 by manandre          #+#    #+#             */
/*   Updated: 2024/12/04 09:07:55 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int c)
{
	char	c_tmp;

	c_tmp = c;
	while (*str != c_tmp)
	{
		if (*str == '\0')
			return (0);
		str ++;
	}
	return ((char *)str);
}
