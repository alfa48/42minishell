/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_signal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 11:39:24 by manandre          #+#    #+#             */
/*   Updated: 2024/11/29 11:39:26 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    signal_handler(int signal)
{
    if (signal == SIGINT)
        g_signal = signal;
}

void    check_signal()
{
    signal(SIGINT, signal_handler);  // Captura Ctrl+C
    signal(SIGTSTP, signal_handler); // Captura Ctrl+Z
}