/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manandre <manandre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 08:32:29 by manandre          #+#    #+#             */
/*   Updated: 2025/01/30 18:34:14 by manandre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_simple_cmd_src(char *s1, char *s2, char *s3)
{
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	if (s3)
		free(s3);
}

void	cmd_not_found_end_exit(char *str)
{
	char	*fw;

	fw = get_first_word(str);
	if (is_entirely_within_quotes(str) || is_within_quotes(str, fw))
	{
		cmd_not_found(fw);
		free(fw);
		free(str);
		exit(14);
	}
	else
		free(fw);
}

void	simple_cmd(char *str, t_cmd *cmd)
{
	int		pid;
	char	*heredoc_delim;
	char	*clean_cmd;
	char	*tmp;

	heredoc_delim = NULL;
	clean_cmd = NULL;
	cmd->pid_count++;
	pid = fork();
	if (pid == 0)
	{
		heredoc_delim = get_heredoc_delimiter(str);
		close(cmd->pipefd[1]);
		configure_stdin(heredoc_delim, cmd->pipefd);
		cmd->redirects = parse_redirects(str, cmd);
		handle_redirects(cmd->redirects);
		clean_cmd = prepare_command(str, heredoc_delim);
		tmp = get_first_word(clean_cmd);
		if (ft_strcmp(tmp, "echo") == 0)
		{
			mini_echo(clean_cmd, str);
			free_simple_cmd_src(clean_cmd, heredoc_delim, tmp);
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
