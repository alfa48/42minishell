#include "minishell.h"

void	prepare_for_execution(t_node *node, char **env, t_cmd *cmd)
{
    free(node->right->command);
    node->right->command = ft_strdup("heredoc_file.txt");
    exec_redin(node, env, cmd);
}

void	write_to_file(char *result)
{
    int fd = open("heredoc_file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, result, ft_strlen(result));
    free(result);
}

void	add_line_to_result(char *line, char *result, size_t *buffer_size, size_t *total_size)
{
    size_t needed = *total_size + ft_strlen(line) + 2;
    if (needed > *buffer_size) {
        *buffer_size = needed * 2;
        char *new_buffer = realloc(result, *buffer_size);
        if (!new_buffer)
        {
            free(result);
            free(line);
            return;
        }
        result = new_buffer;
    }

    strcat(result, line);
    strcat(result, "\n");
    *total_size = strlen(result);
    rl_replace_line("", 0);
    rl_redisplay();
}

void process_input(t_node *node, char *result, size_t buffer_size, size_t total_size)
{
    char *line;
    while (1) {
        int fd_t = open("/dev/tty", O_RDONLY);
        if (fd_t == -1) {
            perror("Erro ao abrir /dev/tty");
            exit(1);
        }

        dup2(fd_t, STDIN_FILENO);
        close(fd_t);

        if (!(line = readline("heredoc> "))) {
            free(result);
            return;
        }

        if (strcmp(line, node->right->command) == 0) {
            free(line);
            break;
        }

        add_line_to_result(line, result, &buffer_size, &total_size);
        free(line);
    }
}

void	exec_heredoc(t_node *node, char **env, t_cmd *cmd)
{
    char *result = NULL;
    size_t total_size = 0;
    size_t buffer_size = 1024;
    pid_t pid;

    result = malloc(buffer_size);
    result[0] = '\0';
    pid = fork();

    if (pid == 0)
    {
        process_input(node, result, buffer_size, total_size);
        write_to_file(result);
        prepare_for_execution(node, env, cmd);
    }
    waitpid(pid, NULL, 0);
}

