/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:01:26 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/30 23:28:27 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_signal_status = 0;

int	is_only_spaces(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] > 32)
			return (0);
	}
	return (1);
}

void	keep_on_shell(t_cmd *cmd)
{
	cmd->line = expanding(cmd->line, cmd);
	cmd->root = init_shell(cmd->line);
	if (cmd->root && cmd->line)
	{
		init_args_next(cmd);
		if (cmd->size == 1)
			mini_built_in(cmd, &(cmd->g_env_list));
		else
			exec(cmd);
	}
}

int	main(void)
{
	t_cmd	*cmd;
	int		saved_stdin;

	cmd = init_before_init();
	saved_stdin = dup(STDIN_FILENO);
	while (1)
	{
		init_args_ofen(cmd);
		cmd->line = readline("minishell$> ");
		add_history(cmd->line);
		set_sig_status_cmd(cmd);
		if (!cmd->line)
			return (write_exit(cmd));
		if (!is_only_spaces(cmd->line))
		{
			if (checks_str(cmd))
				continue ;
			keep_on_shell(cmd);
		}
		dup2(saved_stdin, STDIN_FILENO);
		free_one_iterator(cmd);
	}
	return (0);
}
