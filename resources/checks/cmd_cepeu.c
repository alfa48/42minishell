/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cepeu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:04:22 by fjilaias          #+#    #+#             */
/*   Updated: 2025/01/30 20:24:24 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_first_word_export(char *command)
{
	char	*fw;
	int		rs;

	rs = 0;
	fw = get_first_word(command);
	if (ft_strcmp(fw, "export") == 0)
		rs = 1;
	free(fw);
	return (rs);
}

static int	process_quote_char(char *init, bool *in_s_q, bool *in_d_q)
{
	if (*init == '\'' && !*in_d_q)
	{
		*in_s_q = !*in_s_q;
		return (1);
	}
	else if (*init == '"' && !*in_s_q)
	{
		*in_d_q = !*in_d_q;
		return (1);
	}
	return (0);
}

static char	*process_spaces(char *arg, char **out, bool in_s_q, bool in_d_q)
{
	while (*arg == ' ' && !in_s_q && !in_d_q)
	{
		arg++;
		if (*arg && *arg != ' ')
		{
			**out = ' ';
			(*out)++;
		}
	}
	return (arg);
}

char	*process_cmd(char *arg)
{
	char	*out;
	char	*tmp;
	bool	in_s_q;
	bool	in_d_q;

	in_s_q = false;
	in_d_q = false;
	if (is_first_word_export(arg))
		return (ft_strdup(arg));
	out = malloc((ft_strlen(arg) + 2));
	if (!out)
		return (NULL);
	tmp = out;
	while (*arg)
	{
		arg = process_spaces(arg, &out, in_s_q, in_d_q);
		if (!process_quote_char(arg, &in_s_q, &in_d_q))
			*out++ = *arg;
		if (*arg)
			arg++;
	}
	*out = '\0';
	return (tmp);
}
