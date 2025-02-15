# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abelov <abelov@student.42london.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/05 00:46:36 by abelov            #+#    #+#              #
#    Updated: 2023/12/05 00:46:38 by abelov           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME		=	libftprintf.a
LIBTMP		=	libtmp.a
CC			=	clang
CFLAGS		=	-g3 -Wall -Wextra -Werror -iquote $(INC_DIR)/
AR			=	ar
ARFLAGS		=	rcs
LIBFT_PATH	=	./lib/libft
LIBFT		=	$(LIBFT_PATH)/libft.a
RM 			:=	rm -rf

SRC_FILES	:= ft_printf.c \
				ft_vprintf.c \
				ft_print_unsigned.c \
				ft_print_hex.c \
				ft_printf_utils1.c \
				ft_printf_utils2.c \
				ft_printf_utils3.c \
				ft_printf_utils4.c

INC_DIR		=	include
OBJ_DIR		=	obj
SRC_DIR		=	src
OBJS		=	$(SRC_FILES:%.c=$(OBJ_DIR)/%.o)

all:				$(NAME)

$(OBJ_DIR)/%.o:		$(SRC_DIR)/%.c
					$(CC) $(CFLAGS) -c $< -o $@

$(NAME):			$(LIBFT) $(OBJ_DIR) $(OBJS)
					$(AR) $(ARFLAGS) $(OBJ_DIR)/$(LIBTMP) $(OBJS)
					$(CC) -nostdlib -r -o $(NAME) $(OBJ_DIR)/ft_printf.o \
						-Wl,--no-whole-archive $(OBJ_DIR)/$(LIBTMP) $(LIBFT)

$(LIBFT):
					make -C $(LIBFT_PATH) all

$(OBJ_DIR):
					mkdir -pv $(OBJ_DIR)

$(OBJ_DIR)/main.o:	tests/main.c $(OBJ_DIR)
					$(CC) $(CFLAGS) -c $< -o $@

main:				$(OBJ_DIR)/main.o $(NAME) $(LIBFT)
					$(CC) -lm $^ -o $@

clean:
					make -C $(LIBFT_PATH) clean
					$(RM) $(OBJ_DIR) a.out main

fclean:				clean
					make -C $(LIBFT_PATH) fclean
					$(RM) $(NAME)

re:					fclean all main

norm:
					@norminette $(SRC_FILES:%.c=$(SRC_DIR)/%.c) $(INC_DIR)/*
					make -C $(LIBFT_PATH) norm

.PHONY:				all bonus clean fclean re libft norm
