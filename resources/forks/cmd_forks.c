//          <<<<<<<<< ADD CABECALHO >>>>>>>>>

#include "minishell.h"

void	wait_forks(t_cmd *cmd)
{
	int	i;
	int	status;
	int	exit_status;

	i = 0;
	while (i < cmd->pid_count)
	{
		// Processo pai: aguarda os do processos filhos
		waitpid(-1, &status, 0);
		if (WIFEXITED(status))
		{
			exit_status = WEXITSTATUS(status);
			if (exit_status == 1)
				cmd->status_cmd = 127;
			else if (exit_status == 0)
				cmd->status_cmd = 0;
			else
				cmd->status_cmd = 126;
		}
		i ++;
	}
}

void	fork_exec_cmd(t_cmd *cmd, t_node *node)
{
	int		pid;
        char	*path;
        char	**args;
        char	*clean_cmd;
        char	*heredoc_delim;

	pid = fork();
	cmd->pid_count ++;
	if (pid == 0)
	{
		clean_cmd = node->command;
		// Check for heredoc first
		heredoc_delim = get_heredoc_delimiter(node->command);
		if (heredoc_delim)
		{
			handle_heredoc(heredoc_delim, STDIN_FILENO);
			free(heredoc_delim);
		}
		if (heredoc_delim)
			clean_cmd = remove_heredoc(node->command);
		path = find_executable(get_first_word(ft_strdup(clean_cmd)), &(cmd->g_env_list));
		args = get_args(clean_cmd);
		if (execve(path, args, cmd->envl) == -1)
		{
			cmd_not_found(get_first_word(ft_strdup(clean_cmd)));
			free(path);
			exit(1);//exit(errno);
        	}
	}
}
