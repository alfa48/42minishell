/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:14:34 by manandre          #+#    #+#             */
/*   Updated: 2024/11/19 13:51:17 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<unistd.h>

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned int	i;
	unsigned int	sum_s1;
	unsigned int	sum_s2;

	i = 0;
	sum_s1 = 0;
	sum_s2 = 0;
	while (*(s1 + i) != '\0' || *(s2 + i) != '\0')
	{
		sum_s1 = s1[i];
		sum_s2 = s2[i];
		if ((sum_s1 - sum_s2) != 0)
			return (sum_s1 - sum_s2);
		i++;
	}	
	return (sum_s1 - sum_s2);
}
