/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 08:32:29 by manandre          #+#    #+#             */
/*   Updated: 2025/01/27 07:50:09 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_internal_command(char *cmd, const char *command)
{
    return (ft_strncmp(cmd, command, ft_strlen(command)) == 0);
}

void simple_cmd(char *str, t_cmd *cmd)
{
    int	pid;
    char	*heredoc_delim = NULL;
    char	*clean_cmd = NULL;
    t_redirect	**redirects;


    cmd->pid_count++;
    pid = fork();
    if (pid == -1)
        return;
    printf("S CMD - %s\n", str);
    if (pid == 0)
    {
	heredoc_delim = get_heredoc_delimiter(str);
	close(cmd->pipefd[1]);
        configure_stdin(heredoc_delim, cmd->pipefd);
        redirects = parse_redirects(str, cmd);
        handle_redirects(redirects);
        clean_cmd = prepare_command(str, heredoc_delim);
        if (is_internal_command(clean_cmd, "echo"))
        {
        	mini_echo(clean_cmd, str);
        	exit(0);
        }
        execute_with_args(clean_cmd, redirects, cmd);
    }
}

void	exec(t_cmd *cmd)
{
	pipe(cmd->pipefd);
	if (ft_strchr(cmd->line1, PIPE))
		execute_commands(cmd->size, cmd);
	else if (has_redirect(cmd->line1))
		simple_cmd(cmd->line1, cmd);
	close(cmd->pipefd[0]);
	close(cmd->pipefd[1]);
	wait_forks(cmd);
}
