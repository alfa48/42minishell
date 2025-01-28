# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: manandre <manandre@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/15 08:24:27 by manandre          #+#    #+#              #
#
#    Updated: 2025/01/21 08:35:21 by manandre         ###   ########.fr        #
#
#    Updated: 2025/01/21 09:28:26 by fjilaias         ###   ########.fr        #
#									       #
#                                                                              #
# **************************************************************************** #

NAME = minishell
RSC_DIR = ./resources
SRC = $(RSC_DIR)/main/main.c \
	$(RSC_DIR)/init/init_vars.c \
	$(RSC_DIR)/init/init_shell.c \
	$(RSC_DIR)/init/init_shell_utils.c \
	$(RSC_DIR)/init/init_before.c \
	$(RSC_DIR)/utils/mini_utils.c \
	$(RSC_DIR)/utils/mini_utils_2.c \
	$(RSC_DIR)/utils/mini_utils_3.c \
	$(RSC_DIR)/utils/mini_utils_4.c \
	$(RSC_DIR)/utils/mini_utils_5.c \
	$(RSC_DIR)/utils/mini_utils_6.c \
	$(RSC_DIR)/utils/mini_utils_7.c \
	$(RSC_DIR)/mini/cmd_envexit.c \
	$(RSC_DIR)/mini/mini_buit_ins.c \
	$(RSC_DIR)/mini/mini_execv.c \
	$(RSC_DIR)/mini/utils.c \
	$(RSC_DIR)/signals/cmd_signal.c \
	$(RSC_DIR)/path/path_utils.c \
	$(RSC_DIR)/forks/cmd_forks.c \
	$(RSC_DIR)/rediret/redirect.c \
	$(RSC_DIR)/heredoc/utils.c \
	$(RSC_DIR)/heredoc/utils_2.c \
	$(RSC_DIR)/heredoc/utils_3.c \
	$(RSC_DIR)/checks/checks.c \
	$(RSC_DIR)/checks/cmd_cepeu.c \
	$(RSC_DIR)/parsers/parsers_cmd.c \
	$(RSC_DIR)/parsers/first_handlers.c\
	$(RSC_DIR)/expand/mini_expand.c \
	$(RSC_DIR)/expand/expand_utils.c \
	$(RSC_DIR)/exec/mini_exec.c \
	$(RSC_DIR)/exec/exec_single_commands_utils.c \
	$(RSC_DIR)/exec/exec_commands.c \
	$(RSC_DIR)/exec/exec_utils.c \
	$(RSC_DIR)/exec/exec_utils_2.c \
	$(RSC_DIR)/exec/exec_pipe_redirectrs.c \
	$(RSC_DIR)/pipe/exec_pipe_left.c \
	$(RSC_DIR)/pipe/exec_pipe_middle.c \
	$(RSC_DIR)/pipe/exec_pipe_right.c \
	$(RSC_DIR)/pipe/utils_pipe.c \

OBJ = $(SRC:.c=.o)

LIBFT_DIR = ./libft
LIBFT_A = $(LIBFT_DIR)/libft.a

CCFLAGS = cc -Wall -Wextra -Werror

all:	$(NAME)

$(NAME) : $(OBJ) $(LIBFT_A)
	$(CCFLAGS) $(OBJ) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)

%.o: %.c
	$(CCFLAGS) -I$(LIBFT_DIR) -Iheaders -c $< -o $@

clean:
	rm -f $(OBJ)
	$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C $(LIBFT_DIR)

$(LIBFT_A):
		$(MAKE) -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re
