/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_redirectrs.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 14:37:29 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/27 11:10:12 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Inicializa o estado do redirecionamento
static void	init_redirect_state(t_redirect_state *state, char *write_ptr,
		const char *read_ptr)
{
	state->in_quotes = false;
	state->in_redirect = false;
	state->quote_char = 0;
	state->write_ptr = write_ptr;
	state->read_ptr = read_ptr;
}

// Atualiza o estado de aspas durante a leitura
static void	update_quote_state(t_redirect_state *state)
{
	if (*(state->read_ptr) == '"' || *(state->read_ptr) == '\'')
	{
		if (!state->in_quotes)
		{
			state->in_quotes = true;
			state->quote_char = *(state->read_ptr);
		}
		else if (state->quote_char == *(state->read_ptr))
		{
			state->in_quotes = false;
		}
	}
}

// Ignora caracteres de redirecionamento e o nome do arquivo
static void	skip_redirect(t_redirect_state *state)
{
	while (*(state->read_ptr) && (*(state->read_ptr) == '<'
			|| *(state->read_ptr) == '>'))
		state->read_ptr++;
	while (*(state->read_ptr) && mini_isspace(*(state->read_ptr)))
		state->read_ptr++;
	while (*(state->read_ptr) && !mini_isspace(*(state->read_ptr)))
		state->read_ptr++;
	while (*(state->read_ptr) && mini_isspace(*(state->read_ptr)))
		state->read_ptr++;
}

// Remove espaços extras no início e no final
static void	trim_spaces(char *result)
{
	char	*write_ptr;
	char	*start;

	write_ptr = result + strlen(result) - 1;
	while (write_ptr >= result && mini_isspace(*write_ptr))
		write_ptr--;
	*(write_ptr + 1) = '\0';
	start = result;
	while (*start && mini_isspace(*start))
		start++;
	if (start != result)
		ft_memmove(result, start, strlen(start) + 1);
}

// Função principal para remover redirecionamentos
char	*remove_redirects(const char *cmd)
{
	char				*result;
	t_redirect_state	state;

	result = malloc(ft_strlen(cmd) + 1);
	init_redirect_state(&state, result, cmd);
	while (*(state.read_ptr))
	{
		update_quote_state(&state);
		if (state.in_quotes)
		{
			*(state.write_ptr++) = *(state.read_ptr++);
			continue ;
		}
		if (is_redirect_char(*(state.read_ptr)) && !state.in_redirect)
		{
			state.in_redirect = true;
			skip_redirect(&state);
			state.in_redirect = false;
			continue ;
		}
		*(state.write_ptr++) = *(state.read_ptr++);
	}
	*(state.write_ptr) = '\0';
	trim_spaces(result);
	return (result);
}
