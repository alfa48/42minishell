/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 01:26:48 by manandre          #+#    #+#             */
/*   Updated: 2024/05/24 12:16:52 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*s_cpy;
	size_t	j;

	s_cpy = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!s_cpy)
		return (NULL);
	j = 0;
	while (s[j])
	{
		s_cpy[j] = s[j];
		j++;
	}
	s_cpy[j] = '\0';
	return (s_cpy);
}
