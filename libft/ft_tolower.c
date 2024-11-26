/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 11:59:34 by manandre          #+#    #+#             */
/*   Updated: 2024/05/21 08:05:23 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_uppercase(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (42);
	return (0);
}

int	ft_tolower(int ch)
{
	if (ft_isalpha(ch) && is_uppercase(ch))
	{
		ch += 32;
	}
	return (ch);
}
