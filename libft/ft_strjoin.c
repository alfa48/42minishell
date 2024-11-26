/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 21:58:04 by manandre          #+#    #+#             */
/*   Updated: 2024/05/20 22:36:52 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new_str;
	size_t	s1_len;
	size_t	s2_len;

	if (!s1 && !s2)
		return (0);
	else
	{
		if (!s1)
			return (ft_strdup(s2));
		else if (!s2)
			return (ft_strdup(s1));
	}
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	if ((int)(s1_len + s2_len) < 0)
		return (0);
	new_str = (char *)malloc(sizeof(char) * (s1_len + s2_len) + 1);
	if (!new_str)
		return (0);
	ft_strlcpy(new_str, s1, s1_len + 1);
	ft_strlcpy(new_str + s1_len, s2, s2_len + 1);
	return (new_str);
}
