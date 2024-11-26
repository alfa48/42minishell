/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 08:51:06 by manandre          #+#    #+#             */
/*   Updated: 2024/05/21 09:06:52 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_putnbr_rec_fd(int n, int fd)
{
	if (n >= 10)
	{
		ft_putnbr_rec_fd(n / 10, fd);
		ft_putnbr_rec_fd(n % 10, fd);
	}
	else
		ft_putchar_fd(n + 48, fd);
}

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
	{
		ft_putstr_fd("-2147483648", fd);
		return ;
	}
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		n = -n;
	}
	ft_putnbr_rec_fd(n, fd);
}
