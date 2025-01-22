/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 12:24:48 by manandre          #+#    #+#             */
/*   Updated: 2025/01/22 09:44:20 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*mini_strcat(char *dest, const char *src)
{
	char    *p_dest;

	p_dest = dest;
	while (*p_dest != '\0')
		p_dest ++;
	while (*src != '\0')
	{
		*p_dest = *src;
		p_dest ++;
		src ++;
	}
	*p_dest = '\0';
	return (dest);
}

static int	get_args_size(char **cmd_args)
{
	int	size;

	size = 0;
	while (cmd_args[size] != NULL)
		size ++;
	return (size);
}

char	**get_args(char *cmd)
{
	char	**cmd_args;
	char	**execve_args;
	int		i;

	i = 0;
    if (!is_entirely_within_quotes(cmd))
    {

        cmd_args = ft_split(cmd, ' ');
        execve_args = malloc((get_args_size(cmd_args) + 1) * sizeof(char *));
        //execve_args[0] =  ft_strjoin("/bin/", cmd_args[0]);
        execve_args[0] =  cmd_args[0];
        while (++i < get_args_size(cmd_args))
            execve_args[i] = cmd_args[i];
        execve_args[i] = NULL;
    }
    else
    {
        execve_args = malloc(2 * sizeof(char *));
        execve_args[0] = process_cmd(cmd);
        execve_args[1] = NULL;
    }
    return (execve_args);

}


void execute_tree(t_node *root, t_cmd *cmd)//apagar
{
	(void) root;
	(void) cmd;
}



int is_operator(char *str)
{
    if (!str)
        return 0;

    char *operators[] = {">", "<", ">>", "<<", "|"};
    size_t num_operators = sizeof(operators) / sizeof(operators[0]);
    size_t i = 0;

    while (i < num_operators) {
        if (ft_strcmp(str, operators[i]) == 0)
            return 1;
        i++;
    }

    return 0;
}

int is_redirect(char *str)
{
    if (!str)
        return 0;

    char *operators[] = {">", "<", ">>", "<<"};
    size_t num_operators = sizeof(operators) / sizeof(operators[0]);
    size_t i = 0;
    while (i < num_operators) {
        if (ft_strcmp(str, operators[i]) == 0)
            return 1;
        i++;
    }

    return 0;
}

// Função auxiliar para verificar precedência de redirecionamentos
int get_redirect_priority(char *type) {
    if (ft_strcmp(type, "<") == 0) return 1;  // Input tem prioridade mais alta
    if (ft_strcmp(type, ">") == 0) return 2;  // Output normal
    if (ft_strcmp(type, ">>") == 0) return 2; // Append tem mesma prioridade que output
    return 0;
}

// Função para ordenar redirecionamentos por precedência
void sort_redirects(t_redirect **redirects) {
    int i, j;
    t_redirect *temp;

    if (!redirects) return;

    for (i = 0; redirects[i] && redirects[i + 1]; i++) {
        for (j = 0; redirects[j] && redirects[j + 1]; j++) {
            if (get_redirect_priority(redirects[j]->type) >
                get_redirect_priority(redirects[j + 1]->type)) {
                temp = redirects[j];
                redirects[j] = redirects[j + 1];
                redirects[j + 1] = temp;
            }
        }
    }
}
