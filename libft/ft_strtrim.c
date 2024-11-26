/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 00:20:09 by manandre          #+#    #+#             */
/*   Updated: 2024/05/21 01:16:46 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *s2)
{
	size_t	start;
	size_t	end;

	if (!s1 || !s2)
		return (0);
	if (!*s1 || !*s2)
		return (ft_strdup(s1));
	start = 0;
	while (s1[start] && ft_strchr(s2, s1[start]))
		start++;
	end = ft_strlen(s1) + 1;
	while (end > start && ft_strchr(s2, s1[end - 1]))
		end--;
	return (ft_substr(s1, start, (end - start)));
}
