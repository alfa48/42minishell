/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 12:27:16 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/27 12:32:40 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_swap(void *a, void *b, size_t size)
{
	unsigned char	temp;
	unsigned char	*p1;
	unsigned char	*p2;
	size_t			i;

	i = 0;
	p1 = (unsigned char *)a;
	p2 = (unsigned char *)b;
	while (i < size)
	{
		temp = p1[i];
		p1[i] = p2[i];
		p2[i] = temp;
		i++;
	}
}

void	ft_qsort(void *base, size_t num, size_t size,
		int (*compare)(const void *, const void *))
{
	char	*array;
	void	*pivot;
	size_t	i;

	if (num < 2)
		return ;
	array = (char *)base;
	pivot = array + (num - 1) * size;
	i = 0;
	for (size_t j = 0; j < num - 1; j++)
	{
		if (compare(array + j * size, pivot) < 0)
		{
			ft_swap(array + i * size, array + j * size, size);
			i++;
		}
	}
	ft_swap(array + i * size, pivot, size);
	ft_qsort(array, i, size, compare);
	ft_qsort(array + (i + 1) * size, num - i - 1, size, compare);
}
