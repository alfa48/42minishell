/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 08:32:29 by manandre          #+#    #+#             */
/*   Updated: 2025/01/27 11:50:42 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_not_found_end_exit(char *str)
{
	char	*fw;

	fw = get_first_word(str);
	if (is_entirely_within_quotes(str) || is_within_quotes(str, fw))
	{
		cmd_not_found(fw);
		free(fw);
		exit(0);
	}
}

void	simple_cmd(char *str, t_cmd *cmd)
{
	int		pid;
	char	*heredoc_delim;
	char	*clean_cmd;

	heredoc_delim = NULL;
	clean_cmd = NULL;
	cmd->pid_count++;
	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
	{
		heredoc_delim = get_heredoc_delimiter(str);
		close(cmd->pipefd[1]);
		configure_stdin(heredoc_delim, cmd->pipefd);
		cmd->redirects = parse_redirects(str, cmd);
		handle_redirects(cmd->redirects);
		clean_cmd = prepare_command(str, heredoc_delim);
		if (ft_strcmp(get_first_word(clean_cmd), "echo") == 0)
		{
			mini_echo(clean_cmd, str);
			exit(0);
		}
		execute_with_args(clean_cmd, cmd->redirects, cmd);
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
