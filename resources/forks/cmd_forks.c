//          <<<<<<<<< ADD CABECALHO >>>>>>>>>

#include "minishell.h"

void fork_crt_env_vars(t_cmd *cmd)
{
    int     pid;

    pid = fork();
    if (pid == 0)
    {
        mini_val(cmd->line, cmd);
        list_env_vars(cmd->val_only);
        //geral_free(cmd);// TODO
        exit(EXIT_SUCCESS);
    }
    waitpid(pid, NULL, 0);
    cmd->status_cmd = 0;
}

void	wait_forks(t_cmd *cmd)
{
	int i = 0;
    int status;
    // printf("DEBUG: command not found\n");
	while (i < cmd->pid_count)
	{
    // Processo pai: aguarda os do processos filhos
        waitpid(-1, &status, 0);
        if (WIFEXITED(status))
        {
            int exit_status = WEXITSTATUS(status);
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
void    fork_exec_cmd(t_cmd *cmd, t_node *node)
{
        int pid;
        char *path;
        char **args;

		pid = fork();
        cmd->pid_count++;
		if (pid == 0)
		{
			path = find_executable(get_first_word(ft_strdup(node->command)), &(cmd->g_env_list));
            args = get_args(node->command);

			if (execve(path, args, cmd->envl) == -1)
			{
                cmd_not_found(get_first_word(ft_strdup(node->command)));
				free(path);
				exit(EXIT_FAILURE);
        	}
		}
}
/*RECONSIDERAR*/
void    fork_exec_cmd_(int pos, t_cmd *cmd)
{
        int pid;
        char *path;
        char **args;

        cmd->pid_count++;
		pid = fork();
		if (pid == 0)
		{
			path = find_executable(get_first_word(ft_strdup(cmd->array[pos])), &(cmd->g_env_list));
            args = get_args(cmd->array[pos]);
			if (execve(path, args, cmd->envl) == -1)
			{
				free(path);
				exit(EXIT_FAILURE);
        	}
		}
}