/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 08:32:29 by manandre          #+#    #+#             */
/*   Updated: 2025/01/24 16:44:35 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*void simple_cmd(char *str, t_cmd *cmd)
{
	int			pid;
	char		*heredoc_delim = NULL;
	char		*clean_cmd = NULL;
	t_redirect	**redirects;
    //ft_putstr_fd("Apenas Debug \n", 2);
	cmd->pid_count++;
	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
	{
		heredoc_delim = get_heredoc_delimiter(str);
		close(cmd->pipefd[1]);
		configure_stdin(heredoc_delim, cmd->pipefd);
		redirects = parse_redirects(str, cmd);
		handle_redirects(redirects);
		clean_cmd = prepare_command(str, heredoc_delim);
		execute_with_args(clean_cmd, redirects, cmd);
	}
}*/


// Função auxiliar para verificar comando interno
int is_internal_command(char *cmd, const char *command)
{
    return strncmp(cmd, command, strlen(command)) == 0;
}

// Função para extrair argumento do echo
char *extract_echo_arg(char *cmd)
{
    // Pula "echo" e espaços
    cmd += 4;
    while (*cmd && *cmd <= 32)
        cmd++;
    return cmd;
}

void simple_cmd(char *str, t_cmd *cmd)
{
    int pid;
    char *heredoc_delim = NULL;
    char *clean_cmd = NULL;
    t_redirect **redirects;

    cmd->pid_count++;
    pid = fork();
    if (pid == -1)
        return;

    if (pid == 0)
    {
        // Processa o delimitador do heredoc
        heredoc_delim = get_heredoc_delimiter(str);
        
        // Fecha pipe de escrita
        close(cmd->pipefd[1]);
        
        // Configura stdin para heredoc
        configure_stdin(heredoc_delim, cmd->pipefd);
        
        // Processa redirecionamentos
        redirects = parse_redirects(str, cmd);
        handle_redirects(redirects);
        
        // Limpa comando
        clean_cmd = prepare_command(str, heredoc_delim);
        
        // Verifica se é um comando interno como echo
        if (is_internal_command(clean_cmd, "echo"))
        {
            char *arg = extract_echo_arg(clean_cmd);
			printf("ARG - %s\n", arg);
            mini_echo(arg, str);
            exit(0);
        }
        
        // Executa comando externo
        execute_with_args(clean_cmd, redirects, cmd);
    }
}

void	exec(t_cmd *cmd)
{
	pipe(cmd->pipefd);
	if (ft_strchr(cmd->line1, PIPE))
		execute_commands(cmd->size, cmd);
	else if (has_redirect(cmd->line1))
		{
			//simple_cmd(cmd->line1, cmd);
			simple_cmd(cmd->line1, cmd);
		}
	close(cmd->pipefd[0]);
	close(cmd->pipefd[1]);
	wait_forks(cmd);
}
