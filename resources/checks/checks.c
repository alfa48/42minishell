/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 09:08:22 by manandre          #+#    #+#             */
/*   Updated: 2025/01/27 10:56:35 by fjilaias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_last(char *str, int size, int *rs, char *patterns)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
		i++;
	i--;
	j = 0;
	while (j < size)
	{
		if (str[i] == patterns[j])
			*rs = 1;
		j++;
	}
	if (*rs)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (1);
	}
	return (0);
}

static int	checks_start_end(char *str)
{
	int		i;
	int		rs;
	int		size;
	char	patterns[3];

	if (!str || !*str)
		return (0);
	i = 0;
	rs = 0;
	patterns[0] = '<';
	patterns[1] = '>';
	patterns[2] = '|';
	size = sizeof(patterns) / sizeof(patterns[0]);
	while (i < size)
	{
		if (str[0] == patterns[i])
		{
			rs = 1;
			break ;
		}
		i++;
	}
	if (check_last(str, size, &rs, patterns))
		return (1);
	return (0);
}

static int	handle_token_check(char *str, int *i, int *space, char *sms_error)
{
	char	c;

	if (str[*i] && (str[*i] == '<' || str[*i] == '>' || str[*i] == '|'))
	{
		if (is_within_quotes(str, str + *i))
		{
			(*i)++;
			return (0);
		}
		c = str[*i];
		(*i)++;
		if (c == '>' && str[*i] == '>')
			(*i)++;
		else if (c == '<' && str[*i] == '<')
			(*i)++;
		*space = 0;
		while (str[*i] && str[*i] <= 32)
		{
			(*i)++;
			*space = 1;
		}
		if (!str[*i] || str[*i] == '<' || str[*i] == '>' || str[*i] == '|')
			return ((0 * printf("%s `%c'\n", sms_error, c)) + 1);
	}
	return (0);
}

int	checks_error_pattern(char *str)
{
	char	*sms;
	int		i;
	int		space;

	i = 0;
	space = 0;
	sms = "minishell: syntax error near unexpected token";
	while (str[i])
	{
		while (str[i] && str[i] <= 32)
			i++;
		if (handle_token_check(str, &i, &space, sms))
		{
			return (1);
		}
		if (str[i])
			i++;
	}
	return (0);
}

int	checks_str(t_cmd *cmd)
{
	char	*str;

	str = ft_strdup(cmd->line);
	if (!str)
		return (1);
	if (checks_start_end(str) || checks_error_pattern(str))
		return (1);
	free(str);
	return (0);
}
