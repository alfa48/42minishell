/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_classification.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:08:03 by fjilaias          #+#    #+#             */
/*   Updated: 2024/11/12 11:09:11 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <linux/limits.h>

typedef struct	s_CommandNode
{
    char	*command; // O comando em si (NULL para nós que representam operadores)
    char	operator; // '|', '>', '<' etc. (usado em nós intermediários)
    struct s_CommandNode	*left;  // Comando à esquerda (entrada)
    struct s_CommandNode	*right; // Comando à direita (saída)
}	t_CommandNode;


int	has_pipe(char *command)
{
	return (strchr(command, '|') != NULL);
}

int	has_redirection(char *command)
{
	return (strchr(command, '>') != NULL || strchr(command, '<') != NULL);
}

int	is_simple_command(char *command)
{
	return !has_pipe(command) && !has_redirection(command);
}

t_CommandNode* parse_command(char *command)
{
    // Verifica se a linha contém um pipe
    char *pipe_position = strchr(command, '|');
    if (pipe_position) {
        *pipe_position = '\0'; // Divide a linha em duas partes
        t_CommandNode *node = malloc(sizeof(t_CommandNode));
        node->command = NULL;
        node->operator = '|';
        node->left = parse_command(command);  // Parte antes do pipe
        node->right = parse_command(pipe_position + 1); // Parte depois > do pipe
        return node;
    }

    // Verifique redirecionadores (simples exemplo para `>`)
    char *redir_position = strchr(command, '>');
    if (redir_position) {
        *redir_position = '\0';
        t_CommandNode *node = malloc(sizeof(t_CommandNode));
        node->command = NULL;
        node->operator = '>';
        node->left = parse_command(command);
        node->right = parse_command(redir_position + 1);
        return node;
    }

    // Caso base: comando simples (nó folha)
    t_CommandNode *node = malloc(sizeof(t_CommandNode));
    node->command = strdup(command);
    node->operator = '\0';
    node->left = node->right = NULL;
    return node;
}

t_CommandNode*	analyze_command(char *command)
{
	t_CommandNode	*Node;

	Node = NULL;
    if (has_pipe(command))
    {
        Node = parse_command(command);
        if (Node)
        {
            Node->left = analyze_command(Node->left->command);
            Node->right = analyze_command(Node->right->command);
        }
    }
    else if (has_redirection(command))
    {
        Node = parse_command(command);
        if (Node)
        {
            Node->left = analyze_command(Node->left->command);
            if (Node->right)
                Node->right = analyze_command(Node->right->command);
        }
    }
    else if (is_simple_command(command))
    {
        Node = parse_command(command);
    }

    return Node;
}

// Função para imprimir a árvore de forma recursiva
void print_command_tree(t_CommandNode *node, int level) {
    if (node == NULL)
        return;

    // Indentar a saída para mostrar a profundidade do nó
    for (int i = 0; i < level; i++) {
        printf("  ");
    }

    // Imprimir o comando ou operador
    if (node->command != NULL) {
        printf("Command: %s\n", node->command);
    } else {
        printf("Operator: %c\n", node->operator);
    }

    // Chamada recursiva para imprimir os filhos (subárvores)
    print_command_tree(node->left, level + 1);
    print_command_tree(node->right, level + 1);
}

int main() {
    char command[] = "ls -l | grep txt > output.txt";
    t_CommandNode *root = analyze_command(command);

    // Imprime a árvore de comandos, começando da raiz
    /*if (root) {
        print_command_tree(root, 0);
    }*/
}

/*
void execute_command_tree(CommandNode *node) {
    if (!node)
    	return ;

    if (node->operator == '|') {
        // Execute a lógica de pipe entre node->left e node->right
    } else if (node->operator == '>') {
        // Lógica para redirecionador
    } else {
        // Execute comando simples usando execvp() ou similar
    }
}

*/

