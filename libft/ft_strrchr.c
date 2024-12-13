/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:34:08 by manandre          #+#    #+#             */
/*   Updated: 2024/12/13 10:20:41 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	int		len;
	char	*str_temp;

	len = ft_strlen(str);
	str_temp = (char *) str;
	while (len >= 0)
	{
		if (str_temp[len] == (unsigned char)c)
			return (&str_temp[len]);
		len--;
	}
	if (!c)
		return (&str_temp[len]);
	return (NULL);
}
