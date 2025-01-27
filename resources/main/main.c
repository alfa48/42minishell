/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:01:26 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/27 16:51:43 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_signal_status = 0;

static void    handle_sigint(int sig)
{
    (void)sig;
    g_signal_status = 1;
    write(1, "\n", 1);
    if (waitpid(-1, NULL, WNOHANG) == -1)  // Se retornar -1, estamos no processo pai
    {
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void	inorder_traversal(t_node *root)
{
	if (root == NULL)
		return ;
	inorder_traversal(root->left);
	if (root->operator)
		printf("Operator:%s$\n", (root->operator));
	else
		printf("Command:%s$\n", root->command);
	inorder_traversal(root->right);
}

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
		inorder_traversal(cmd->root);
		init_args_next(cmd);
		if (cmd->size == 1)
			mini_built_in(cmd, &(cmd->g_env_list));
		else
			exec(cmd);
		free_tree(cmd->root);
	}
	free_ms(cmd);
}

int    main(void)
{
      t_cmd    *cmd;
    struct sigaction    sa;
    
    // Configurar o sigaction antes do loop principal
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handle_sigint;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    
    cmd = init_before_init();
    while (1)
    {
        init_args_ofen(cmd);
        cmd->line = readline("minishell$> ");
        set_sig_status_cmd(cmd);
        if (!cmd->line)
            return (0 * write(1, "exit\n", 5) + 1);
            
        if (!is_only_spaces(cmd->line))
        {
            add_history(cmd->line);
            if (checks_str(cmd))
                continue;
            keep_on_shell(cmd);
        }
        free(cmd->line);
    }
    return (0);
}
