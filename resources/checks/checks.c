/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 09:08:22 by manandre          #+#    #+#             */
/*   Updated: 2025/01/20 14:42:34 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	checks_start_end(char *str)
{
	int	i;
	int	j;
	int	rs;
	int	size;

	rs = 0;
	if (!str || !*str)
		return (0);
	char patterns[] = {'<', '>', '|'};
	size = sizeof(patterns) / sizeof(patterns[0]);
	i = 0;
	// Verifica o primeiro caractere
	while (i < size)
	{
		// "" ->34; ' ->39
		if (str[0] == patterns[i])
		{
			rs = 1;
			break;
		}
		i ++;
	}
	// Encontrar o ultimo caractere não nulo
	i = 0;
	while (str[i])
		i ++;
	i--; // Agora i aponta para o último caractere

    // Verifica o último caractere
	j = 0;
	while (j < size)
	{
		if (str[i] == patterns[j])
			rs = 1;
		j ++;
	}
	if (rs)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (1);
	}
	return (0); // Nenhum erro encontrado
}

int checks_error_pattern(char *str)
{
    int     i;
    char        c;
    int     space;

    i = 0;
    while (str[i])
    {
        space = 0;
        while (str[i] && str[i] <= 32) // Pula espaços e caracteres de controle
            i++;
        if (str[i] && (str[i] == '<' || str[i] == '>' || str[i] == '|' || str[i] == '&'))
        {
            if (is_within_quotes(str, str + i)) // Verifica se está dentro de aspas
            {
                i ++;  // Move para o próximo caractere
                continue;
            }
            c = str[i];
            while (str[i] && str[++i] <= 32) // Pula espaços
                space = 1;
            if (!str[i])
                return (0);
            if (!space && ((c == '<' && str[i] == '<') || (c == '>' && str[i] == '>')))
                continue ;
            if (str[i] == '|')
                return (1 + 0 * \
                printf("minishell: syntax error near unexpected token '|'\n"));
            else if (str[i] == '<' || str[i] == '>' || str[i] == '|')
                return (1 + 0 * \
                printf("minishell: syntax error near unexpected token `newline'\n"));
        }
        if (str[i])
            i++;
    }
    return (0);
}

int	checks_str(t_cmd *cmd)
{
	char		*str;

	str = ft_strdup(cmd->line);
	printf("%s\n", str);
	if (!str)
		return (1);
	if (checks_start_end(str) || checks_error_pattern(str))
	 	return (1);
	free(str);
	return (0);
}