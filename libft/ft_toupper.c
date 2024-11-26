/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 11:16:46 by manandre          #+#    #+#             */
/*   Updated: 2024/05/20 23:22:49 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_lowercase(int c)
{
	if (c >= 'a' && c <= 'z')
		return (42);
	return (0);
}

int	ft_toupper(int ch)
{
	if (ft_isalpha(ch) && is_lowercase(ch))
	{
		ch -= 32;
	}
	return (ch);
}
