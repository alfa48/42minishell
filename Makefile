# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fjilaias <fjilaias@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/15 08:24:27 by manandre          #+#    #+#              #
#    Updated: 2024/10/30 15:59:08 by fjilaias         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
RSC_DIR = ./resources
SRC = $(RSC_DIR)/cmd_cepeu.c \
	$(RSC_DIR)/cmd_envexit.c \
	$(RSC_DIR)/main.c \
	$(RSC_DIR)/mini_execv.c \
	$(RSC_DIR)/init/init_vars.c \
	$(RSC_DIR)/utils/heredoc.c \
	$(RSC_DIR)/utils/exec_pipe_redirectrs.c \
	$(RSC_DIR)/utils/mini_utils.c \
	$(RSC_DIR)/utils/mini_utils_2.c \
	$(RSC_DIR)/utils/mini_utils_3.c \
	$(RSC_DIR)/utils/mini_utils_4.c \
	$(RSC_DIR)/init_shell.c \
	$(RSC_DIR)/first_handlers.c\
	$(RSC_DIR)/parsers_cmd.c \
	$(RSC_DIR)/mini_expand.c \
	$(RSC_DIR)/signals/cmd_signal.c \
	$(RSC_DIR)/path/path_utils.c \
	$(RSC_DIR)/forks/cmd_forks.c
	
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
