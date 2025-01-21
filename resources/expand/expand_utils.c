/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 11:22:00 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/21 12:06:08 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strndup(const char *s, size_t n)
{
	size_t	len;
	size_t	i;
	char	*dup;

	len = 0;
	if (!s)
		return (NULL);
	while (s[len] && len < n)
		len++;
	dup = (char *)malloc(len + 1);
	if (!dup)
		return (NULL);
	i = -1;
	while (++i < len)
		dup[i] = s[i];
	dup[len] = '\0';
	return (dup);
}
