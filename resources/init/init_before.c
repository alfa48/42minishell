/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_before.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 16:21:00 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/20 16:21:13 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*init_before_init(void)
{
	extern char	**environ;
	t_cmd		*cmd;

	cmd = malloc(sizeof(t_cmd));
	handle_signals();
	init_args(cmd, environ);
	return (cmd);
}