/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 10:01:21 by manandre          #+#    #+#             */
/*   Updated: 2024/05/21 08:11:42 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	len_tmp;
	char	*substr;

	if (!s)
		return (0);
	if ((unsigned int)ft_strlen(s) < start)
		return (ft_strdup(""));
	len_tmp = ft_strlen(s + start);
	if (len_tmp < len)
		len = len_tmp;
	substr = (char *)malloc((len + 1) * sizeof(char));
	if (!substr)
		return (0);
	ft_strlcpy(substr, s + start, len + 1);
	return (substr);
}
